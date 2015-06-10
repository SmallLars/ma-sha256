{
    // BSIG1(x) = ROTR^6(x) XOR ROTR^11(x) XOR ROTR^25(x)

    SATSolver solver;
    vector<Lit> clause;
    solver.log_to_file("test.log");
    solver.set_num_threads(4);

    uint32_t eingabe = 0xabcdef98;
    uint32_t ausgabe = (eingabe >> 6 | eingabe << (32-6)) ^ (eingabe >> 11 | eingabe << (32-11)) ^ (eingabe >> 25 | eingabe << (32-25));
    uint32_t result = 0;

    Const_32 ca(eingabe);
    ca.append(&solver);

    Bsig1_32 bsig1;
    bsig1.append(&solver);

    lbool ret = solver.solve();
    assert(ret == l_True);

    for (unsigned i = 63; i >=32; i--) {
        result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
    }

    assert(ausgabe == result);
    std::cout << "BSIG1 funktioniert." << std::endl;
}
