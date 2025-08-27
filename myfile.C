void myfile() {
    // Create the RNTuple model
    auto model = ROOT::RNTupleModel::Create();

    // Define four fields: int, float, double, string
    auto intField = model->MakeField<int>("IntField");
    auto floatField = model->MakeField<float>("FloatField");
    auto doubleField = model->MakeField<double>("DoubleField");
    auto stringField = model->MakeField<std::string>("StringField");

    // Set no compression
    auto opts = ROOT::RNTupleWriteOptions();
    opts.SetCompression(0);

    // Create the writer
    auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "Data", "string_output.root", opts);

    // Fill 10 entries
    for (int i = 0; i < 10; ++i) {
        *intField = i;
        *floatField = static_cast<float>(i * i);
        *doubleField = i * 0.5;
        *stringField = "entry_" + std::to_string(i);
        writer->Fill();
    }
    // Writer closes automatically when going out of scope

}


