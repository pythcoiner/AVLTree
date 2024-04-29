# AVLTree
An AVL Tree  implementation in pure c++ using only std library and 256bit key

The key is using the custom uint256 type

example:

    AVLTree tree = AVLTree();*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$**


    uint256 k = tree.random_256(); // for test only

    tree.insert(k, 0x123);
    tree.insert(uint256{0,0,0,5598}, 1245);
    tree.insert(uint256{123456},1111);
    tree.insert(uint256{0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff}, 12);

    Node* ret = tree.search(uint256{0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff});

    std::cout << ret->value << std::endl;
  
output:

    12







