{
    SATSolver solver;
    vector<Lit> clause;
    solver.log_to_file("test.log");
    solver.set_num_threads(4);

    uint32_t eingabe[2] = {1234, 1234};
    uint32_t ausgabe = eingabe[0] + eingabe[1];
    uint32_t result = 0;

    Const_32 ca(eingabe[0]);
    ca.setOutput(0);
    ca.append(&solver);

    ca.setValue(eingabe[1]);
    ca.setOutput(32);
    ca.append(&solver);

    Adder_32 adder;
    adder.append(&solver);

    lbool ret = solver.solve();
    assert(ret == l_True);

    for (unsigned i = 126; i >=95; i--) {
        result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 95));
    }

    assert(ausgabe == result);
    std::cout << "Adder funktioniert." << std::endl;
}
