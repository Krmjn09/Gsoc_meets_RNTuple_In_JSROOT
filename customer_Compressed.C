void customer() {
  using namespace ROOT;
  
  // 1. Define the model with various customer fields
  auto model = RNTupleModel::Create();
  
  // Basic customer info
  auto customerId = model->MakeField<UInt_t>("customerId");
  auto age = model->MakeField<UChar_t>("age");
  
  // Create string fields with proper column representatives using split index
  auto firstName = std::make_unique<RField<std::string>>("firstName");
  firstName->SetColumnRepresentatives({{ENTupleColumnType::kSplitIndex32, ENTupleColumnType::kChar}});
  model->AddField(std::move(firstName));
  
  auto lastName = std::make_unique<RField<std::string>>("lastName");
  lastName->SetColumnRepresentatives({{ENTupleColumnType::kSplitIndex32, ENTupleColumnType::kChar}});
  model->AddField(std::move(lastName));
  
  auto email = std::make_unique<RField<std::string>>("email");
  email->SetColumnRepresentatives({{ENTupleColumnType::kSplitIndex64, ENTupleColumnType::kChar}});
  model->AddField(std::move(email));
  
  // Financial data
  auto accountBalance = model->MakeField<Double_t>("accountBalance");
  auto creditScore = model->MakeField<UShort_t>("creditScore");
  auto monthlyIncome = model->MakeField<Float_t>("monthlyIncome");
  
  // Transaction data
  auto lastTransactionAmount = model->MakeField<Double_t>("lastTransactionAmount");
  auto loyaltyPoints = model->MakeField<Int_t>("loyaltyPoints");
  
  // Boolean flags
  auto isActive = model->MakeField<Bool_t>("isActive");
  auto isPremium = model->MakeField<Bool_t>("isPremium");
  
  
  // Geographic data
  auto zipCode = model->MakeField<UInt_t>("zipCode");
  auto countryCode = model->MakeField<UShort_t>("countryCode");

  auto opts = ROOT::RNTupleWriteOptions();
opts.SetMaxUnzippedClusterSize(1024); // 1 KiB per cluster (normally would be ~1GB)
  
  // 3. Create the writer
  auto writer = RNTupleWriter::Recreate(std::move(model), "CustomerData", "customer_Compressed.root", opts);
  
  // Get pointers to string fields
  auto firstNamePtr = writer->GetModel().GetDefaultEntry().GetPtr<std::string>("firstName");
  auto lastNamePtr = writer->GetModel().GetDefaultEntry().GetPtr<std::string>("lastName");
  auto emailPtr = writer->GetModel().GetDefaultEntry().GetPtr<std::string>("email");
  
  // Sample data arrays for realistic customer generation
  std::vector<std::string> firstNames = {
    "John", "Jane", "Michael", "Sarah", "David", "Emily", "Robert", "Jessica",
    "William", "Ashley", "Christopher", "Amanda", "Matthew", "Stephanie", "Daniel"
  };
  
  std::vector<std::string> lastNames = {
    "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis",
    "Rodriguez", "Martinez", "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson"
  };
  
  std::vector<std::string> domains = {
    "gmail.com", "yahoo.com", "hotmail.com", "outlook.com", "company.com"
  };
  
  std::vector<UInt_t> sampleZipCodes = {
    10001, 90210, 60601, 30301, 75201, 98101, 02101, 33101, 80201, 85001
  };
  
  TRandom3 rng;
  
  // 4. Fill with sample customer data
  for (int i = 0; i < 100; ++i) {
    // Basic info
    *customerId = i + 1000; // Start IDs from 1000
    *age = 18 + rng.Integer(65); // Age between 18-82
    
    // Random names
    *firstNamePtr = firstNames[rng.Integer(firstNames.size())];
    *lastNamePtr = lastNames[rng.Integer(lastNames.size())];
    
    // Generate email
    std::string emailStr = *firstNamePtr + "." + *lastNamePtr + "@" + domains[rng.Integer(domains.size())];
    std::transform(emailStr.begin(), emailStr.end(), emailStr.begin(), ::tolower);
    *emailPtr = emailStr;
    
    // Financial data
    *accountBalance = rng.Gaus(5000, 2000); // Mean $5000, std $2000
    if (*accountBalance < 0) *accountBalance = 0; // No negative balances
    
    *creditScore = 300 + rng.Integer(551); // Credit score 300-850
    *monthlyIncome = 2000 + rng.Exp(3000); // Exponential distribution for income
    
    // Transaction data
    *lastTransactionAmount = rng.Gaus(100, 50); // Mean $100, std $50
    *loyaltyPoints = rng.Integer(10000); // 0-9999 points
    
    // Boolean flags
    *isActive = rng.Rndm() > 0.1; // 90% active customers
    *isPremium = rng.Rndm() > 0.8; // 20% premium customers
    
    
    // Geographic
    *zipCode = sampleZipCodes[rng.Integer(sampleZipCodes.size())];
    *countryCode = 1 + rng.Integer(195); // Country codes 1-195
    
    writer->Fill();
  }
  
  std::cout << "Generated 100 customer records in customer.root" << std::endl;
}
