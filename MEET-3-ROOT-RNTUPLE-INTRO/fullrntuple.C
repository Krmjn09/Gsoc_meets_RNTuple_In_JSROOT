void fullrntuple() {
    // Create the RNTuple model
    auto model = ROOT::RNTupleModel::Create();

    // Define fields in the schema
    auto field1 = model->MakeField<int>("field1");
    auto field2 = model->MakeField<float>("field2");

    // Create the writer: name = "name", file = "output.root"
    auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "name", "final.root");

    // Fill data
    for (int i = 0; i < 100; ++i) {
        *field1 = i;
        *field2 = i * i;
        writer->Fill();
    }

    // Optional but good practice
    writer->CommitCluster();
}
