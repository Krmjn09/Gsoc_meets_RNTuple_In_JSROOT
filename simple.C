using namespace ROOT;

void student_records_rntuple() {
    std::cout << "Starting Student Records RNTuple creation..." << std::endl;
    
    // 1. Define the model for student records
    auto model = RNTupleModel::Create();
    std::cout << "Student model created successfully." << std::endl;
    
    // Basic student information with automatic ColumnType mapping
    model->MakeField<bool>("IsEnrolled");           // ENTupleColumnType::kBit
    model->MakeField<std::byte>("GradeLetter");     // ENTupleColumnType::kByte (A=65, B=66, etc.)
    model->MakeField<char>("SectionCode");          // ENTupleColumnType::kChar
    model->MakeField<std::int8_t>("AttendanceWeek");    // ENTupleColumnType::kInt8 (week number)
    model->MakeField<std::uint8_t>("CourseCredits");    // ENTupleColumnType::kUInt8
    
    // Student identifiers and counts with automatic mapping
    model->MakeField<std::int16_t>("StudentID");        // ENTupleColumnType::kInt16
    model->MakeField<std::uint16_t>("EnrollmentYear");  // ENTupleColumnType::kUInt16
    model->MakeField<std::int32_t>("UniversityID");     // ENTupleColumnType::kInt32
    model->MakeField<std::uint32_t>("LibraryCardNum");  // ENTupleColumnType::kUInt32
    model->MakeField<std::int32_t>("SSN");              // ENTupleColumnType::kInt32 (using 32-bit for JS compatibility)
    model->MakeField<std::uint32_t>("StudentLoanNum");  // ENTupleColumnType::kUInt32 (using 32-bit for JS compatibility)
    
    // Academic scores with automatic mapping
    model->MakeField<float>("GPA");                     // ENTupleColumnType::kReal32
    model->MakeField<double>("CumulativeGPA");          // ENTupleColumnType::kReal64
    
    // Split integer types for compressed storage
    auto fSplitStudentRank = std::make_unique<RField<std::int16_t>>("StudentRank");
    fSplitStudentRank->SetColumnRepresentatives({{ENTupleColumnType::kSplitInt16}});
    model->AddField(std::move(fSplitStudentRank));
    
    auto fSplitClassSize = std::make_unique<RField<std::uint16_t>>("ClassSize");
    fSplitClassSize->SetColumnRepresentatives({{ENTupleColumnType::kSplitUInt16}});
    model->AddField(std::move(fSplitClassSize));
    
    auto fSplitTestScore = std::make_unique<RField<std::int32_t>>("TestScore");
    fSplitTestScore->SetColumnRepresentatives({{ENTupleColumnType::kSplitInt32}});
    model->AddField(std::move(fSplitTestScore));
    
    auto fSplitTotalPoints = std::make_unique<RField<std::uint32_t>>("TotalPoints");
    fSplitTotalPoints->SetColumnRepresentatives({{ENTupleColumnType::kSplitUInt32}});
    model->AddField(std::move(fSplitTotalPoints));
    
    auto fSplitStudyHours = std::make_unique<RField<std::int32_t>>("StudyHours");
    fSplitStudyHours->SetColumnRepresentatives({{ENTupleColumnType::kSplitInt32}});
    model->AddField(std::move(fSplitStudyHours));
    
    auto fSplitTuitionPaid = std::make_unique<RField<std::uint32_t>>("TuitionPaid");
    fSplitTuitionPaid->SetColumnRepresentatives({{ENTupleColumnType::kSplitUInt32}});
    model->AddField(std::move(fSplitTuitionPaid));
    
    // Split floating point types for compressed academic data
    auto fSplitQuizAverage = std::make_unique<RField<float>>("QuizAverage");
    fSplitQuizAverage->SetColumnRepresentatives({{ENTupleColumnType::kSplitReal32}});
    model->AddField(std::move(fSplitQuizAverage));
    
    auto fSplitWeightedGPA = std::make_unique<RField<double>>("WeightedGPA");
    fSplitWeightedGPA->SetColumnRepresentatives({{ENTupleColumnType::kSplitReal64}});
    model->AddField(std::move(fSplitWeightedGPA));
    
    // String fields with different index types for student information
    auto fStudentName = std::make_unique<RField<std::string>>("StudentName");
    fStudentName->SetColumnRepresentatives({{ENTupleColumnType::kSplitIndex64, ENTupleColumnType::kChar}});
    model->AddField(std::move(fStudentName));

    auto fMajor = std::make_unique<RField<std::string>>("Major");
    fMajor->SetColumnRepresentatives({{ENTupleColumnType::kSplitIndex32, ENTupleColumnType::kChar}});
    model->AddField(std::move(fMajor));

    auto fEmailAddress = std::make_unique<RField<std::string>>("EmailAddress");
    fEmailAddress->SetColumnRepresentatives({{ENTupleColumnType::kIndex64, ENTupleColumnType::kChar}});
    model->AddField(std::move(fEmailAddress));

    auto fCity = std::make_unique<RField<std::string>>("City");
    fCity->SetColumnRepresentatives({{ENTupleColumnType::kIndex32, ENTupleColumnType::kChar}});
    model->AddField(std::move(fCity));
    
    // 2. Create the writer
    std::cout << "Creating student records writer..." << std::endl;
    auto writer = RNTupleWriter::Recreate(std::move(model),
                                         "StudentRecords",
                                         "student_records.root");
    std::cout << "Writer created successfully." << std::endl;
    
    // 3. Bind pointers via default entry
    std::cout << "Binding field pointers..." << std::endl;
    auto &entry = writer->GetModel().GetDefaultEntry();
    
    // Basic student information
    auto pIsEnrolled = entry.GetPtr<bool>("IsEnrolled");
    auto pGradeLetter = entry.GetPtr<std::byte>("GradeLetter");
    auto pSectionCode = entry.GetPtr<char>("SectionCode");
    auto pAttendanceWeek = entry.GetPtr<std::int8_t>("AttendanceWeek");
    auto pCourseCredits = entry.GetPtr<std::uint8_t>("CourseCredits");
    
    // Student identifiers
    auto pStudentID = entry.GetPtr<std::int16_t>("StudentID");
    auto pEnrollmentYear = entry.GetPtr<std::uint16_t>("EnrollmentYear");
    auto pUniversityID = entry.GetPtr<std::int32_t>("UniversityID");
    auto pLibraryCardNum = entry.GetPtr<std::uint32_t>("LibraryCardNum");
    auto pSSN = entry.GetPtr<std::int32_t>("SSN");
    auto pStudentLoanNum = entry.GetPtr<std::uint32_t>("StudentLoanNum");
    
    // Academic scores
    auto pGPA = entry.GetPtr<float>("GPA");
    auto pCumulativeGPA = entry.GetPtr<double>("CumulativeGPA");
    
    // Split compressed fields
    auto pStudentRank = entry.GetPtr<std::int16_t>("StudentRank");
    auto pClassSize = entry.GetPtr<std::uint16_t>("ClassSize");
    auto pTestScore = entry.GetPtr<std::int32_t>("TestScore");
    auto pTotalPoints = entry.GetPtr<std::uint32_t>("TotalPoints");
    auto pStudyHours = entry.GetPtr<std::int32_t>("StudyHours");
    auto pTuitionPaid = entry.GetPtr<std::uint32_t>("TuitionPaid");
    auto pQuizAverage = entry.GetPtr<float>("QuizAverage");
    auto pWeightedGPA = entry.GetPtr<double>("WeightedGPA");
    
    // String fields with different index types
    auto pStudentName = entry.GetPtr<std::string>("StudentName");
    auto pMajor = entry.GetPtr<std::string>("Major");
    auto pEmailAddress = entry.GetPtr<std::string>("EmailAddress");
    auto pCity = entry.GetPtr<std::string>("City");
    
    // 4. Fill student records
    std::cout << "Starting to fill student records..." << std::endl;
    for (int i = 0; i < 100; ++i) {
        // Basic student information
        *pIsEnrolled = (i % 10 != 0);  // Most students enrolled, some dropped
        *pGradeLetter = (std::byte)(65 + (i % 5));  // Grades A-E (A=65, B=66, etc.)
        *pSectionCode = 'A' + (i % 8);  // Section codes A-H
        *pAttendanceWeek = static_cast<std::int8_t>((i % 16) + 1);  // Week 1-16
        *pCourseCredits = static_cast<std::uint8_t>((i % 6) + 1);   // 1-6 credits
        
        // Student identifiers
        *pStudentID = static_cast<std::int16_t>(1000 + i);
        *pEnrollmentYear = static_cast<std::uint16_t>(2020 + (i % 5));  // 2020-2024
        *pUniversityID = 100000 + i * 13;  // Unique university IDs
        *pLibraryCardNum = 500000 + i * 7;  // Library card numbers
        *pSSN = 123456 + i * 1000;     // Mock SSN (32-bit for JS compatibility)
        *pStudentLoanNum = 900000 + i * 12345;  // Student loan numbers (32-bit)
        
        // Academic scores
        *pGPA = 2.0f + (3.0f * (i % 21) / 20.0f);  // GPA between 2.0-5.0
        *pCumulativeGPA = 2.2 + (2.8 * (i % 29) / 28.0);  // Cumulative GPA 2.2-5.0
        
        // Split compressed academic data (same values, different storage)
        *pStudentRank = static_cast<std::int16_t>((100 - i) + (i % 50)); // Class rank
        *pClassSize = static_cast<std::uint16_t>(25 + (i % 75));  // Class size 25-100
        *pTestScore = 65 + (i % 36);  // Test scores 65-100
        *pTotalPoints = 850 + (i % 151);  // Total points 850-1000
        *pStudyHours = i * 25 + (i % 100);  // Weekly study hours (32-bit)
        *pTuitionPaid = 15000 + (i * 500) + (i % 20000);  // Tuition paid (32-bit)
        
        // Split floating point academic data
        *pQuizAverage = 70.0f + (25.0f * (i % 31) / 30.0f);  // Quiz avg 70-95
        *pWeightedGPA = 2.5 + (2.0 * (i % 26) / 25.0);  // Weighted GPA 2.5-4.5
        
        // String fields with realistic student data
        *pStudentName = "Student_" + std::to_string(1000 + i) + "_" + 
                       std::string(1, 'A' + (i % 26)) + std::string(1, 'A' + ((i*7) % 26));
        
        // Majors array for variety
        std::vector<std::string> majors = {
            "Computer Science", "Mathematics", "Physics", "Chemistry", "Biology",
            "Engineering", "Literature", "History", "Psychology", "Economics"
        };
        *pMajor = majors[i % majors.size()];
        
        *pEmailAddress = "student" + std::to_string(1000 + i) + "@university.edu";
        
        // Cities array for variety
        std::vector<std::string> cities = {
            "New York", "Los Angeles", "Chicago", "Houston", "Phoenix",
            "Philadelphia", "San Antonio", "San Diego", "Dallas", "San Jose"
        };
        *pCity = cities[i % cities.size()];
                
        writer->Fill(); // Commit student record
        
        if (i % 20 == 0) {
            std::cout << "Processed student record " << i << " (ID: " << (1000 + i) << ")" << std::endl;
        }
        
        if (i == 50) {
            std::cout << "Committing cluster after 50 student records..." << std::endl;
            writer->CommitCluster(); // Commit cluster every 50 students
        }
    }
    
    std::cout << "Finalizing student records..." << std::endl;
    // Force writer to be destroyed/finalized
    writer.reset();
    
    std::cout << "Successfully wrote student_records.root with 100 student records and all column types." << std::endl;
}