void myreader(){
    //open the rntuple
    auto reader = ROOT::RNTupleReader::Open("name","output.root");
    //get views for each field
    auto view1 = reader->GetView<int>("field1");
    auto view2 = reader->GetView<float>("field2");

    //looping over data
    for(auto x:*reader){
        reader->LoadEntry(x);
        int val1= view1(x);
        float val2 = view2(x);
        std::cout << "Entry " << x
                  << ": field1 = " << val1
                  << ", field2 = " << val2
                  << std::endl;
    }
}