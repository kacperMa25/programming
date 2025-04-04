#include <iostream>
#include <string>

using namespace std;

class Solution {
  public:
    string gcdOfStrings(string str1, string str2) {
        int lenghtDev = str2.length();
        string gcd;
        for (int i = 0; (i + lenghtDev) <= str1.length(); i += lenghtDev) {
        }
        return gcd;
    }
};

int main() {
    Solution sol;
    string str1 = "ABCABC", str2 = "ABC";

    cout << sol.gcdOfStrings(str1, str2) << endl;
}
