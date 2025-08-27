void myreader_string() {
    // Open the RNTuple
    auto reader = ROOT::RNTupleReader::Open("Data", "string_output.root");
    if (!reader) {
        std::cerr << "Failed to open RNTuple!" << std::endl;
        return;
    }

    reader->PrintInfo();  // Optional: print schema info

    // Get view for the string field
    auto view = reader->GetView<std::string>("field");

    // Loop over entries
    for (auto i : *reader) {
        reader->LoadEntry(i);  // optional, forces loading
        std::cout << "Entry " << i << ": field = " << view(i) << std::endl;
    }
}
