{
    // BSIG0(x) = ROTR^2(x) XOR ROTR^13(x) XOR ROTR^22(x)

    SATSolver solver;
    vector<Lit> clause;
    solver.log_to_file("test.log");
    solver.set_num_threads(4);

    uint32_t eingabe = 0xabcdef98;
    uint32_t ausgabe = (eingabe >> 2 | eingabe << (32-2)) ^ (eingabe >> 13 | eingabe << (32-13)) ^ (eingabe >> 22 | eingabe << (32-22));
    uint32_t result = 0;

    Const_32 ca(eingabe);
    ca.append(&solver);

    Bsig0_32 bsig0;
    bsig0.append(&solver);

    lbool ret = solver.solve();
    assert(ret == l_True);

    for (unsigned i = 63; i >=32; i--) {
        result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
    }

    assert(ausgabe == result);
    std::cout << "BSIG0 funktioniert." << std::endl;
}
