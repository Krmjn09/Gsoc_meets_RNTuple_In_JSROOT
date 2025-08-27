using namespace ROOT;

void simple() {    
    // 1. Create model with basic field types
    auto model = RNTupleModel::Create();
    
    // Basic types with automatic ColumnType mapping
    model->MakeField<bool>("flag");                 // ENTupleColumnType::kBit
    model->MakeField<std::byte>("byte_val");        // ENTupleColumnType::kByte
    model->MakeField<char>("char_val");             // ENTupleColumnType::kChar
    model->MakeField<std::int8_t>("int8_val");      // ENTupleColumnType::kInt8
    model->MakeField<std::uint8_t>("uint8_val");    // ENTupleColumnType::kUInt8
    
    // Integer types
    model->MakeField<std::int16_t>("int16_val");    // ENTupleColumnType::kInt16
    model->MakeField<std::uint16_t>("uint16_val");  // ENTupleColumnType::kUInt16
    model->MakeField<std::int32_t>("int32_val");    // ENTupleColumnType::kInt32
    model->MakeField<std::uint32_t>("uint32_val");  // ENTupleColumnType::kUInt32
    model->MakeField<std::int32_t>("int64_val");    // ENTupleColumnType::kInt32 (32-bit for JS)
    model->MakeField<std::uint32_t>("uint64_val");  // ENTupleColumnType::kUInt32 (32-bit for JS)
    
    // Floating point types
    model->MakeField<float>("float_val");           // ENTupleColumnType::kReal32
    model->MakeField<double>("double_val");         // ENTupleColumnType::kReal64
    
    // Split integer types for compression
    auto split_int16 = std::make_unique<RField<std::int16_t>>("split_int16");
    split_int16->SetColumnRepresentatives({{ENTupleColumnType::kSplitInt16}});
    model->AddField(std::move(split_int16));
    
    auto split_uint16 = std::make_unique<RField<std::uint16_t>>("split_uint16");
    split_uint16->SetColumnRepresentatives({{ENTupleColumnType::kSplitUInt16}});
    model->AddField(std::move(split_uint16));
    
    auto split_int32 = std::make_unique<RField<std::int32_t>>("split_int32");
    split_int32->SetColumnRepresentatives({{ENTupleColumnType::kSplitInt32}});
    model->AddField(std::move(split_int32));
    
    auto split_uint32 = std::make_unique<RField<std::uint32_t>>("split_uint32");
    split_uint32->SetColumnRepresentatives({{ENTupleColumnType::kSplitUInt32}});
    model->AddField(std::move(split_uint32));
    
    // Split floating point types
    auto split_float = std::make_unique<RField<float>>("split_float");
    split_float->SetColumnRepresentatives({{ENTupleColumnType::kSplitReal32}});
    model->AddField(std::move(split_float));
    
    auto split_double = std::make_unique<RField<double>>("split_double");
    split_double->SetColumnRepresentatives({{ENTupleColumnType::kSplitReal64}});
    model->AddField(std::move(split_double));
    
    // String fields with different index types
    auto str_split64 = std::make_unique<RField<std::string>>("str_split64");
    str_split64->SetColumnRepresentatives({{ENTupleColumnType::kSplitIndex64, ENTupleColumnType::kChar}});
    model->AddField(std::move(str_split64));

    auto str_split32 = std::make_unique<RField<std::string>>("str_split32");
    str_split32->SetColumnRepresentatives({{ENTupleColumnType::kSplitIndex32, ENTupleColumnType::kChar}});
    model->AddField(std::move(str_split32));

    auto str_index64 = std::make_unique<RField<std::string>>("str_index64");
    str_index64->SetColumnRepresentatives({{ENTupleColumnType::kIndex64, ENTupleColumnType::kChar}});
    model->AddField(std::move(str_index64));

    auto str_index32 = std::make_unique<RField<std::string>>("str_index32");
    str_index32->SetColumnRepresentatives({{ENTupleColumnType::kIndex32, ENTupleColumnType::kChar}});
    model->AddField(std::move(str_index32));
    
    // 2. Create writer
    auto opts = ROOT::RNTupleWriteOptions();
   opts.SetCompression(0);
    auto writer = RNTupleWriter::Recreate(std::move(model),
                                         "TestData",
                                         "uncompressed_test_data.root");
    std::cout << "Writer created successfully." << std::endl;
    
    // 3. Get field pointers
    auto &entry = writer->GetModel().GetDefaultEntry();
    
    // Basic type pointers
    auto p_flag = entry.GetPtr<bool>("flag");
    auto p_byte = entry.GetPtr<std::byte>("byte_val");
    auto p_char = entry.GetPtr<char>("char_val");
    auto p_int8 = entry.GetPtr<std::int8_t>("int8_val");
    auto p_uint8 = entry.GetPtr<std::uint8_t>("uint8_val");
    
    // Integer type pointers
    auto p_int16 = entry.GetPtr<std::int16_t>("int16_val");
    auto p_uint16 = entry.GetPtr<std::uint16_t>("uint16_val");
    auto p_int32 = entry.GetPtr<std::int32_t>("int32_val");
    auto p_uint32 = entry.GetPtr<std::uint32_t>("uint32_val");
    auto p_int64 = entry.GetPtr<std::int32_t>("int64_val");
    auto p_uint64 = entry.GetPtr<std::uint32_t>("uint64_val");
    
    // Floating point pointers
    auto p_float = entry.GetPtr<float>("float_val");
    auto p_double = entry.GetPtr<double>("double_val");
    
    // Split type pointers
    auto p_split_int16 = entry.GetPtr<std::int16_t>("split_int16");
    auto p_split_uint16 = entry.GetPtr<std::uint16_t>("split_uint16");
    auto p_split_int32 = entry.GetPtr<std::int32_t>("split_int32");
    auto p_split_uint32 = entry.GetPtr<std::uint32_t>("split_uint32");
    auto p_split_float = entry.GetPtr<float>("split_float");
    auto p_split_double = entry.GetPtr<double>("split_double");
    
    // String type pointers
    auto p_str_split64 = entry.GetPtr<std::string>("str_split64");
    auto p_str_split32 = entry.GetPtr<std::string>("str_split32");
    auto p_str_index64 = entry.GetPtr<std::string>("str_index64");
    auto p_str_index32 = entry.GetPtr<std::string>("str_index32");
    
    // 4. Fill test data
    for (int i = 0; i < 100; ++i) {
        // Basic types
        *p_flag = (i % 2 == 0);
        *p_byte = (std::byte)(i % 256);
        *p_char = 'A' + (i % 26);
        *p_int8 = static_cast<std::int8_t>(i % 128);
        *p_uint8 = static_cast<std::uint8_t>(i % 256);
        
        // Integer types
        *p_int16 = static_cast<std::int16_t>(i * 100);
        *p_uint16 = static_cast<std::uint16_t>(i * 200);
        *p_int32 = i * 1000;
        *p_uint32 = i * 2000;
        *p_int64 = i * 10000;  // Using 32-bit storage
        *p_uint64 = i * 20000; // Using 32-bit storage
        
        // Floating point types
        *p_float = i * 1.5f;
        *p_double = i * 2.5;
        
        // Split types (same values, different storage)
        *p_split_int16 = static_cast<std::int16_t>(i * 100);
        *p_split_uint16 = static_cast<std::uint16_t>(i * 200);
        *p_split_int32 = i * 1000;
        *p_split_uint32 = i * 2000;
        *p_split_float = i * 1.5f;
        *p_split_double = i * 2.5;
        
        // String types
        *p_str_split64 = "split64_" + std::to_string(i);
        *p_str_split32 = "split32_" + std::to_string(i);
        *p_str_index64 = "index64_" + std::to_string(i);
        *p_str_index32 = "index32_" + std::to_string(i);
        
        writer->Fill();
        
        if (i % 25 == 0) {
            std::cout << "Filled entry " << i << std::endl;
        }
        
        if (i == 50) {
            std::cout << "Committing cluster..." << std::endl;
            writer->CommitCluster();
        }
    }
    
    writer.reset();
    
    std::cout << "Test file 'test_data.root' created with 100 entries and all column types." << std::endl;
}