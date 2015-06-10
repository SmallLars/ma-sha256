{
    // SSIG0(x) = ROTR^7(x) XOR ROTR^18(x) XOR SHR^3(x)

    SATSolver solver;
    vector<Lit> clause;
    solver.log_to_file("test.log");
    solver.set_num_threads(4);

    uint32_t eingabe = 0xabcdef98;
	uint32_t ausgabe = (eingabe >> 7 | eingabe << (32-7)) ^ (eingabe >> 18 | eingabe << (32-18)) ^ (eingabe >> 3);
    uint32_t result = 0;

    Const_32 ca(eingabe);
    ca.append(&solver);

    Ssig0_32 ssig0;
    ssig0.append(&solver);

    lbool ret = solver.solve();
    assert(ret == l_True);

    for (unsigned i = 63; i >=32; i--) {
        result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
    }

    assert(ausgabe == result);
    std::cout << "SSIG0 funktioniert." << std::endl;
}
