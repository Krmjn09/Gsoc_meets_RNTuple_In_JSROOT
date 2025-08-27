void string_writer() {
    // Create the RNTuple model
    auto model = ROOT::RNTupleModel::Create();

    // Define a single string field
    auto field = model->MakeField<std::string>("field");

    // Set no compression
    auto opts = ROOT::RNTupleWriteOptions();
    opts.SetCompression(0);

    // Create the writer
    auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "Data", "string_output.root", opts);

    // Fill 10 string entries
    for (int i = 0; i < 10; ++i) {
        *field = "entry_" + std::to_string(i);
        writer->Fill();
    }

    // Writer closes automatically
}
