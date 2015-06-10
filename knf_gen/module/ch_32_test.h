{
    // CH(a; b; c) = (a AND b) XOR ( (NOT a) AND c)

    SATSolver solver;
    vector<Lit> clause;
    solver.log_to_file("test.log");
    solver.set_num_threads(4);

    uint32_t eingabe[3] = {0xabcdef98, 0x651d8fa1, 0x456af012};
    uint32_t ausgabe = (eingabe[0] & eingabe[1]) ^ ((~eingabe[0]) & eingabe[2]);
    uint32_t result = 0;

    Const_32 ca(eingabe[0]);
    ca.setOutput(0);
    ca.append(&solver);

    ca.setValue(eingabe[1]);
    ca.setOutput(32);
    ca.append(&solver);

    ca.setValue(eingabe[2]);
    ca.setOutput(64);
    ca.append(&solver);

    Ch_32 ch;
    ch.append(&solver);

    lbool ret = solver.solve();
    assert(ret == l_True);

    for (unsigned i = 127; i >=96; i--) {
        result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 96));
    }

    assert(ausgabe == result);
    std::cout << "CH funktioniert." << std::endl;
}
