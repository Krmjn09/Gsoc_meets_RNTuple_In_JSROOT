constexpr char const *kNTupleFileName = "ntpl001_staff.root";

void Ingest()
{
   // The input file cernstaff.dat is in the same directory as this macro
   std::ifstream fin("./cernstaff.dat");
   
   // Check if file opens and provide detailed error
   if (!fin.is_open()) {
      std::cerr << "Error: Cannot open cernstaff.dat in current directory (" << gSystem->pwd() << ")" << std::endl;
      std::cerr << "Ensure cernstaff.dat is in the same directory as ntpl001_staff.C" << std::endl;
      return;
   }
   std::cout << "File opened successfully: ./cernstaff.dat" << std::endl;

   // Create a unique pointer to an empty data model
   auto model = ROOT::RNTupleModel::Create();

   // Define fields
   auto fldCategory = model->MakeField<int>("Category");
   auto fldFlag = model->MakeField<unsigned int>("Flag");
   auto fldAge = model->MakeField<int>("Age");
   auto fldService = model->MakeField<int>("Service");
   auto fldChildren = model->MakeField<int>("Children");
   auto fldGrade = model->MakeField<int>("Grade");
   auto fldStep = model->MakeField<int>("Step");
   auto fldHrweek = model->MakeField<int>("Hrweek");
   auto fldCost = model->MakeField<int>("Cost");
   auto fldDivision = model->MakeField<std::string>("Division");
   auto fldNation = model->MakeField<std::string>("Nation");

   // Create RNTuple writer with no compression
   auto opts = ROOT::RNTupleWriteOptions();
   opts.SetCompression(0);
   auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "Staff", kNTupleFileName, opts);

   // Read and process data
   std::string record;
   int lineCount = 0;
   while (std::getline(fin, record)) {
      std::istringstream iss(record);
      if (iss >> *fldCategory >> *fldFlag >> *fldAge >> *fldService >> *fldChildren >> *fldGrade >> *fldStep >> *fldHrweek >> *fldCost >> *fldDivision >> *fldNation) {
         writer->Fill();
         lineCount++;
      } else {
         std::cerr << "Warning: Failed to parse line " << lineCount + 1 << ": " << record << std::endl;
      }
   }
   std::cout << "Processed " << lineCount << " entries" << std::endl;
   fin.close();
}

void Analyze()
{
   // Get a unique pointer to an empty RNTuple model
   auto model = ROOT::RNTupleModel::Create();

   // Define the field needed for reading
   std::shared_ptr<int> fldAge = model->MakeField<int>("Age");

   // Open the RNTuple
   auto reader = ROOT::RNTupleReader::Open(std::move(model), "Staff", kNTupleFileName);

   // Print ntuple info
   reader->PrintInfo();

   std::cout << "The first entry in JSON format:" << std::endl;
   reader->Show(0);

   // Create histogram
   auto c = new TCanvas("c", "", 200, 10, 700, 500);
   TH1I h("h", "Age Distribution CERN, 1988", 100, 0, 100);
   h.SetFillColor(48);

   for (auto entryId : *reader) {
      reader->LoadEntry(entryId);
      h.Fill(*fldAge);
   }

   h.DrawCopy();
}

void ntpl001_staff()
{
   Ingest();
   Analyze();
}