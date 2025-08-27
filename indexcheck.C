


void indexcheck() {
    auto model = RNTupleModel::Create();

    // Default: compressed string → SplitIndex64
    model->AddField<std::string>("splitIndex64");

    // Preserved logic: manually specify Index64
    auto nonSplitString = std::make_unique<RField<std::string>>("index64");
    nonSplitString->SetColumnRepresentatives({{ENTupleColumnType::kIndex64, ENTupleColumnType::kChar}});
    model->AddField(std::move(nonSplitString));

    // Manually force SplitIndex32
    auto splitIndex32 = std::make_unique<RField<std::string>>("splitIndex32");
    splitIndex32->SetColumnRepresentatives({{ENTupleColumnType::kSplitIndex32, ENTupleColumnType::kChar}});
    model->AddField(std::move(splitIndex32));

    // Manually force Index32
    auto index32 = std::make_unique<RField<std::string>>("index32");
    index32->SetColumnRepresentatives({{ENTupleColumnType::kIndex32, ENTupleColumnType::kChar}});
    model->AddField(std::move(index32));

    auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "IndexData", "index_fields.root");

    for (int i = 0; i < 100; ++i) {
        writer->Fill({
            "splitIndex64", "split_" + std::to_string(i),
            "index64", "idx64_" + std::to_string(i),
            "splitIndex32", "split32_" + std::to_string(i),
            "index32", "idx32_" + std::to_string(i)
        });
    }
}
