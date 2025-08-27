void convert_to_uncompressed() {
    constexpr const char* kInputFileName = "ntuple001_staff.root";
    constexpr const char* kOutputFileName = "ntuple001_staff_uncompressed.root";

    // Open original compressed RNTuple
    auto reader = ROOT::RNTupleReader::Open("Staff", kInputFileName);
    if (!reader) {
        std::cerr << "❌ Failed to open input file.\n";
        return;
    }

    // Create new model for writing
    auto model = ROOT::RNTupleModel::Create();
    auto field = model->MakeField<std::string>("field");

    // Disable compression
    auto opts = ROOT::RNTupleWriteOptions();
    opts.SetCompression(0);

    // Create uncompressed writer
    auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "Staff", kOutputFileName, opts);

    // Read original entries and fill into uncompressed writer
    auto view = reader->GetView<std::string>("field");
    for (auto i : *reader) {
        reader->LoadEntry(i);
        *field = view(i);
        writer->Fill();
    }

    std::cout << "✅ Successfully converted to uncompressed file: " << kOutputFileName << std::endl;
}