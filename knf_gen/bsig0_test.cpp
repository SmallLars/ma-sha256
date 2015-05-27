{
    // BSIG0(x) = ROTR^2(x) XOR ROTR^13(x) XOR ROTR^22(x)

    SATSolver solver;
    vector<Lit> clause;
    solver.log_to_file("log_test.txt");
    solver.set_num_threads(4);

    // 32 Eingangsvariablen (0 bis 31) + 32 Ausgangsvariablen (32 bis 63)
    solver.new_vars(64);

    uint32_t a, b;
    a = 0xabcdef98;
    for (unsigned i = 32; i > 0; i--) {
        bool tmp = ((a >> (i - 1) & 1));
        std::cout << tmp;
    }
    std::cout << std::endl;

    Const ca(a);
    ca.append(&solver);
    ca.append("test");

    b = (a >> 2 | a << (32-2)) ^ (a >> 13 | a << (32-13)) ^ (a >> 22 | a << (32-22));
    for (unsigned i = 32; i > 0; i--) std::cout << ((b >> (i - 1) & 1)); std::cout << std::endl;

    Bsig0 bsig0;
    bsig0.append(&solver);
    bsig0.append("test");

    lbool ret = solver.solve();

    for (unsigned i = 63; i >=32; i--) std::cout << (solver.get_model()[i] == l_True? 1 : 0); std::cout << std::endl;
/*
    assert(ret == l_True);
    assert(solver.get_model()[0] == l_True);
    assert(solver.get_model()[1] == l_False);
    assert(solver.get_model()[2] == l_True);

    std::cout
    << "Solution is: "
    << solver.get_model()[0]
    << ", " << solver.get_model()[1]
    << ", " << solver.get_model()[2]
    << std::endl;
*/
}
