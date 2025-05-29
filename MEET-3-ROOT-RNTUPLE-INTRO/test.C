#include <iostream>
void test(int n=42)
{
    std::cout << "Hello from ROOT!" << std::endl;
    //passing parameters directly in the terminal
    std:: cout<<n;
}

// calling macro using "root test.C" in the terminal
// root [0] .x MacroName.C