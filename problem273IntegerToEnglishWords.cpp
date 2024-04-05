#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Solution {
public:
    // Finding the values of digits'.
    int digitNumber ( int n, int *y ) {
        int digit = 0;
        while ( n > 0) {
            y[ digit ] = n % 10; 
            digit++;
            n = n / 10;
        }
        return digit;
    }

    std::string helper( int digit, int *y, std::string z = "" ) {
        std::string birler[] = { "", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen" };
        std::string onlar[] = { "", "Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };
        if ( digit <= 0 ) { // Empty String
            return "";
        }
        else if ( digit == 1 ) {
            if ( y[0] < 0 || y[0] > 9 ) {
                return "Invalid number";
            }
            return ( birler[y[0]] ); // Numbers between 0 and 10
        }
        else if ( digit == 2 ) {
            if ( y[1] < 0 || y[1] > 9 || y[0] < 0 || y[0] > 9 ) {
                return "Invalid number";
            }
            if ( y[1] == 1 ){
                return ( birler[ 10 + y[0] ] );  // Numbers between 10 and 20
            }
            else if ( y[0] == 0 ){
                return ( onlar[ y[1] ] ); // Numbers between 10 and 100 that is x % 10 == 0
            }
            else{
                return ( onlar[ y[1] ] + " " + birler[ y[0] ] ); // Other 2 digit numbers
            }
        }
        else {
            if ( y[2] < 0 || y[2] > 9 || y[1] < 0 || y[1] > 9 || y[0] < 0 || y[0] > 9 ) {
                return "Invalid number";
            }
            else {
                if ( y[1] == 0 && y[2] == 0 ) { // Numbers between 100 and 1000 that is x % 100 == 0
                    return ( birler[ y[0] ] );
                }
                else if ( y[1] == 1 ) { // Numbers between 100 and 1000 that is middle digit is 1
                    return ( birler[ y[2] ] + " " + z + " " + birler[ 10 + y[0] ] );
                }
                return ( birler[ y[2] ] + " " + z + " " + onlar[ y[1] ] + " " + birler[y[0]] ); // Other 3 digit numbers
            }
        }   
    }

    std::string deleteExtraSpaces( std::string result = "" ) {
        // Delete the extra spaces between the words if they exists.
        for ( size_t i = 0; i < result.length() - 1; ++i ) {
            if ( result[i] == ' ' && result[i + 1] == ' ' ) {
                result.erase( i, 1 ); 
            }
        }
        // Delete last indexes if they are space.
        while ( result.back() == ' ' ) {
            result.pop_back();
        }
        return result;
    }

    // Here, we're checking the "Hundred", "Thousand", "Million" and "Billion" words are exists in the result, and if so we're finding their positions.
    std::vector<size_t> findThePositions( int found = 0, std::string result = "", std::string word = "" ){
        std::vector<size_t> positions;
        while ( found != std::string::npos) {
            found = result.find( word, found ); 
            if (found != std::string::npos) {
                positions.push_back(found);
                found += 7; 
            }
            else {
                positions.push_back(-1); // Word not found, assign -1.
            }
        }
        return positions;
    }

    // Here, we're finding the extra word's indexes should be deleted ( Thousand, Hundred etc.. ).
    // Able to do this, we're checking if this words has 1 space between them. 
    // Not all these words combinations are not false but some of them are. We need find the false ones.
    // Example: "Hundred" can't be right after "Thousand". -> "One Thousand Hundred" is false.
    std::vector<std::pair<size_t, size_t>> deleteListMaker( std::vector<std::pair<size_t, size_t>> deleteList, std::vector<size_t> positions1, std::vector<size_t> positions2, int num1, int num2 ){
        for (size_t i = 0; i < positions1.size(); ++i) { 
            for ( size_t j = 0; j < positions2.size(); ++j ) {
                if ( ( positions1[i] - positions2[j] ) == num1 && positions1[i] != -1 && positions2[j] != -1 ) { 
                    deleteList.push_back( std::make_pair( positions1[i], num2 ) );
                }
            }
        }
        return deleteList;
    }

    
    std::string numberToWords( int num ) {

        if ( num == 0 ){
            return "Zero";
        }

        int *y = nullptr;
        std::string result, result1, result2, result3, result4;
        int control = 0;
        y = new int [20]; //Digits' values

        int sayi = num;
        int digit = digitNumber( num, y );
        int left_digit = digit;

        // We'll send the number's last 3 digit, then the other 3 digit...
        // If left digit < 3 than send the remain digits.

        result1 = helper( digit, y, "Hundred" );
        left_digit = left_digit - 3;
        if ( control == 0 && 0 < left_digit && left_digit <= 3 ) { 
            digit = left_digit;
            control = 1; //Checking if the number has 3 digits+ or below ( Thousand )
        }

        result2 = helper( digit, y + 3, "Hundred" );
        left_digit = left_digit - 3; 
        if ( control == 0 && 0 < left_digit && left_digit <= 3 ) { 
            digit = left_digit;
            control = 2; //Checking if the number has 3 digits+ or below ( Million )
        }

        result3 = helper( digit, y + 6, "Hundred" );
        left_digit = left_digit - 3; 
        if ( control == 0 && 0 < left_digit && left_digit <= 3 ) { 
            digit = left_digit;
            control = 3; //Checking if the number has 3 digits+ or below ( Billion )
        }

        result4 = helper( digit, y + 9, "Hundred" );

        if ( control == 0 ) { //Max 3 digits
            result = result1;
        }
        else if ( control == 1 ) { //Max 6 digits
            result = result2 + " Thousand " + result1;
        }
        else if ( control == 2 ) { //Max 9 digits
            result = result3 + " Million " + result2 + " Thousand " + result1;
        }
        else if ( control == 3 ) { //Max 12 digits
            result = result4 + " Billion " + result3 + " Million " + result2 + " Thousand " + result1;
        }

        result = deleteExtraSpaces( result ); //Deleting the extra spaces in the string if the exist.

        // Here, we'll check the "Hundred", "Thousand", "Million" and "Billion" words are exists in the result, and if so we'll find the their positions.
        int found1 = 0, found2 = 0, found3 = 0, found4 = 0;
        std::vector<size_t> positions1, positions2, positions3, positions4;
        positions1 = findThePositions( found1, result, "Hundred" );
        positions2 = findThePositions( found2, result, "Thousand" );
        positions3 = findThePositions( found3, result, "Million" );
        positions4 = findThePositions( found4, result, "Billion" );

        // Here, we're finding the extra word's indexes should be deleted ( Thousand, Hundred etc.. ).
        std::vector<std::pair<size_t, size_t>> deleteList;
        deleteList = deleteListMaker( deleteList, positions1, positions2, 9, 8 ); // Checking for "Thousand Hundred"
        deleteList = deleteListMaker( deleteList, positions1, positions3, 8, 8 ); // Checking for "Million Hundred"
        deleteList = deleteListMaker( deleteList, positions1, positions4, 8, 8 ); // Checking for "Billion Hundred"
        deleteList = deleteListMaker( deleteList, positions2, positions3, 8, 9 ); // Checking for "Million Thousand"
        deleteList = deleteListMaker( deleteList, positions3, positions4, 8, 8 ); // Checking for "Billion Million"

        // After finding the deleted words' indexes should be deleted, we're sorting them decreasing.
        // Because if we delete first indexes first, the others will shift and their indexes will change.
        std::sort( deleteList.begin(), deleteList.end(), []( const auto& a, const auto& b ) {
            return a.first > b.first;
        });

        // Delete words.
        for ( const auto& del : deleteList ) {
            result.erase( del.first, del.second );
        }

        result = deleteExtraSpaces( result );
        return result;
    }
}; 

int main ( void ) {
    Solution sol;
    std::string word = sol.numberToWords( 30205068 );
    std::cout << "\n" << word << "\n";
    return 0;
}