void customerReader() {
    // Open the RNTuple
    auto reader = ROOT::RNTupleReader::Open("CustomerData", "customer.root");
    
    // Check if reader opened successfully
    if (!reader) {
        std::cerr << "Failed to open RNTuple!" << std::endl;
        return;
    }
    
    // Print RNTuple information
    reader->PrintInfo();
    
    // Get views for a few key fields
    auto viewCustomerId = reader->GetView<UInt_t>("customerId");
    auto viewFirstName = reader->GetView<std::string>("firstName");
    auto viewLastName = reader->GetView<std::string>("lastName");
    auto viewEmail = reader->GetView<std::string>("email");
    auto viewAccountBalance = reader->GetView<Double_t>("accountBalance");
    auto viewAge = reader->GetView<UChar_t>("age");
    
    // Loop over data
    for (auto x : *reader) {
        reader->LoadEntry(x);
        
        UInt_t customerId = viewCustomerId(x);
        std::string firstName = viewFirstName(x);
        std::string lastName = viewLastName(x);
        std::string email = viewEmail(x);
        Double_t accountBalance = viewAccountBalance(x);
        UChar_t age = viewAge(x);
        
        std::cout << "Entry " << x
                  << ": Customer ID = " << customerId
                  << ", Name = " << firstName << " " << lastName
                  << ", Email = " << email
                  << ", Age = " << (int)age
                  << ", Balance = $" << accountBalance
                  << std::endl;
    }
}