void mywriter() {
    //creating the model
    auto model = ROOT::RNTupleModel::Create();
    //defining model schema
    auto field1 = model->MakeField<int>("field1");
    auto field2 = model->MakeField<float>("field2");
    auto field3 = model->MakeField<bool>("field3");   // bool field

    //creating a writer
    auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "name", "output.root");

    //filling the data
    for (int i = 0; i < 10; ++i) {
        *field1 = i;
        *field2 = i * i;
        *field3 = (i % 2 == 0);   // true for even numbers
        writer->Fill();
    }
}

void myreader() {
    //open the rntuple
    auto reader = ROOT::RNTupleReader::Open("name", "output.root");
    if (!reader) {
        std::cerr << "Failed to open RNTuple!" << std::endl;
        return;
    }

    reader->PrintInfo();

    auto view1 = reader->GetView<int>("field1");
    auto view2 = reader->GetView<float>("field2");
    auto view3 = reader->GetView<bool>("field3");   // bool view

    //looping over data
    for (auto entryId : *reader) {
        int val1 = view1(entryId);
        float val2 = view2(entryId);
        bool val3 = view3(entryId);

        std::cout << "Entry " << entryId
                  << ": field1 = " << val1
                  << ", field2 = " << val2
                  << ", field3 = " << (val3 ? "true" : "false")
                  << std::endl;
    }
}
