{
    // SSIG1(x) = ROTR^17(x) XOR ROTR^19(x) XOR SHR^10(x)

    SATSolver solver;
    vector<Lit> clause;
    solver.log_to_file("test.log");
    solver.set_num_threads(4);

    uint32_t eingabe = 0xabcdef98;
	uint32_t ausgabe = (eingabe >> 17 | eingabe << (32-17)) ^ (eingabe >> 19 | eingabe << (32-19)) ^ (eingabe >> 10);
    uint32_t result = 0;

    Const_32 ca(eingabe);
    ca.append(&solver);

    Ssig1_32 ssig1;
    ssig1.append(&solver);

    lbool ret = solver.solve();
    assert(ret == l_True);

    for (unsigned i = 63; i >=32; i--) {
        result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
    }

    assert(ausgabe == result);
    std::cout << "SSIG1 funktioniert." << std::endl;
}
