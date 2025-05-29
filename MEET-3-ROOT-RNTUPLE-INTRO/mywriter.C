void mywriter(){
    //creating the model
    auto model = ROOT::RNTupleModel::Create();
    //defining model schema
    auto field1 = model->MakeField<int>("field1");
    auto field2 = model->MakeField<float>("field2");

    //creating a writer
    auto writer = ROOT::RNTupleWriter::Recreate(std::move(model),"name","output.root");
int i;
    //filling the data
    for( i =0;i<100;++i){
    *field1=i;
    *field2=i*i;
    writer->Fill();
    }


}