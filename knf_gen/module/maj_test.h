{
    // MAJ(x; y; z) = (x AND y) XOR (x AND z) XOR (y AND z)

    SATSolver solver;
    vector<Lit> clause;
    solver.log_to_file("log_test.txt");
    solver.set_num_threads(4);

    // 96 Eingangsvariablen (0 bis 95) + 32 Ausgangsvariablen (96 bis 127)
    solver.new_vars(128);

    uint32_t eingabe[3] = {0xabcdef98, 0x651d8fa1, 0x456af012};
    uint32_t ausgabe = (eingabe[0] & eingabe[1]) ^ (eingabe[0] & eingabe[2]) ^ (eingabe[1] & eingabe[2]);
    uint32_t result = 0;

    Const ca(eingabe[0]);
    ca.setStart(0);
    ca.append(&solver);

    ca.setValue(eingabe[1]);
    ca.setStart(32);
    ca.append(&solver);

    ca.setValue(eingabe[2]);
    ca.setStart(64);
    ca.append(&solver);

    Maj maj;
    maj.append(&solver);

    lbool ret = solver.solve();
    assert(ret == l_True);

    for (unsigned i = 127; i >=96; i--) {
        result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 96));
    }

    assert(ausgabe == result);
    std::cout << "MAJ funktioniert." << std::endl;
}
