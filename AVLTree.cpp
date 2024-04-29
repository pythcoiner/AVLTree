#include "AVLTree.h"

// Node methods

void Node::processHeight() {
    /*
     * Process the height calculation of the node based on child height.
     */

    std::int64_t r_height = -1; // right child height
    std::int64_t l_height = -1; // left child height
    if (this->left != nullptr){
        l_height = this->left->height ;
    }
    if (this->right != nullptr){
        r_height = this->right->height;
    }
    this->height = std::max(r_height,l_height) + 1;

}

void Node::checkHeight() {
    /*
     * Trigger the node height calculation then (if node is not root node),
     *  trigger parent height calculation.
     */

    this->processHeight(); // process height calculation

    // if root node
    if (this->is_root){
        this->parent->height = this->height; //parent of root node is AVLTree:tree_root
    }

    else if (!this->is_root) {
        // trigger parent check height process
        if (this->parent != nullptr)this->parent->checkHeight();
    }
}

void Node::processBalance() {
    /*
     * Process balance calculation using child heights, then if balance overpass trigger,
     * start the rebalance process.
     */

    const int TRIGGER = 1;

    std::int64_t r_height = -1; // right child height
    std::int64_t l_height = -1; // left child height
    if (this->left != nullptr)l_height = this->left->height ;
    if (this->right != nullptr)r_height = this->right->height;

    this->balance_factor = (l_height) - (r_height); // balance factor calculation

    if (std::abs(this->balance_factor) > TRIGGER) this->rebalance(); // start rebalance process

    else if (this->parent != nullptr) {
        if (!this->parent->is_tree_root) this->parent->processBalance();
    }
    else std::cout <<"Node::processBalance() => Node::parent == nullptr!" << std::endl;

}

void Node::rebalance() {
//    std::cout << "    Node::rebalance()" << std::endl;

    NodePosition insert_position{};
    insert_position.node = this->parent;
    if (this->parent->left == this)insert_position.position = 0;
    else if (this->parent->right == this)insert_position.position = 1;
    else assert(false && "Wrong insert position");

    // record child balance factor
    int64_t l_balance = -1;
    if (this->left != nullptr) l_balance = this->left->balance_factor;

    int64_t  r_balance = -1;
    if (this->right != nullptr) r_balance = this->right->balance_factor;

    // left heavy
    if (this->balance_factor > 1){

        if (l_balance >= 0) this->LLRotation(insert_position);
        else this->LRRotation(insert_position);

    }

    // right heavy
    else if (this->balance_factor < -1){

        if (r_balance <= 0) this->RRRotation(insert_position);
        else this->RLRotation(insert_position);
    }

}

void Node::RLRotation(NodePosition insert) {

/*
 * Process Right-Left rotation, this append when inserting node C:
 * the rotation is made following 2 steps
 *                 |    1=Right      |   2=Left
 *     A           >      A         >      C
 *      \          >       \        >     / \
 *       B         >        C       >    A   B
 *      /          >         \      >
 *      C          >          B     >
 *
 * @param insert This define the parent of the subtree root node.
 */

    // define A,B,C
    Node* A = this;
    Node* B = A->right;
    Node* C = B->left;
    Node* _root = A->parent;
    if (B == nullptr or C == nullptr) return;

    //step1: right
    if (C->left != nullptr) C->left->parent = A;
    A->right = C->left;
    C->parent = A;

    if (C->right != nullptr) C->right->parent = B;
    B->left = C->right;

    B->parent = C;
    C->right = B;

    //step2: left
    C->parent = A->parent;
    A->parent = C;
    C->left = A;

    // handle root tag
    if(A->is_root) {
        _root->right = C;
        A->is_root = false;
        C->is_root = true;
    }

    // update head node in parent
    if (insert.position == 0) insert.node->left = C;
    else if (insert.position == 1) insert.node->right = C;

    // process height for A,B
    A->processHeight();
    B->processHeight();

    // start checkHeight and balance process
    C->checkHeight();
    C->processBalance();
}

void Node::LLRotation(NodePosition insert) {
/* Process Left rotation, this append when inserting node F:
 * - noted connexions are replaced by + noted connexions
 *         A            >          B
 *       -/ \           >         / \+
 *       B   E          >        C   A
 *      / \-            >       /  +/ \
 *     C   D            >      F   D   E
 *    /                 >
 *   F                  >
 * @param insert This define the parent of the subtree root node.
*/

    // assign A,B,D nodes
    Node* A = this;
    Node* B;
    Node* D;
    Node* _root = A->parent;
    B = this->left;
    D = B->right;
    if (B == nullptr)return;

    /* --------- connect B <=> parent --------- */
    B->parent = A->parent;

    // keep the root tag if rotate root
    if(A->is_root){
        _root->right = B;
        A->is_root = false;
        B->is_root = true;
    }

    // set B as parent child instead of A
    if (insert.position == 0) insert.node->left = B;
    else if (insert.position == 1) insert.node->right = B;

    /* --------- connect A <=> D --------- */
    A->left = D;
    if (D != nullptr)D->parent = A;

    /* --------- connect A <=> B --------- */
    B->right = A ;
    A->parent = B;

    // Check weight & balance
    A->checkHeight();
    A->processBalance();
}

void Node::LRRotation(NodePosition insert) {
/* Process Left-Right rotation, this append when inserting node C:
 * the rotation is made following 2 steps
 *                 |    1=Left      |   2=Right
 *         A       >          A     >      C
 *        /        >         /      >     / \
 *       B         >        C       >    B   A
 *        \        >       /        >
 *         C       >      B         >
 * @param insert This define the parent of the subtree root node.
 */

    // define A,B,C
    Node* A = this;
    Node* B = A->left;
    Node* C = B->right;
    Node *_root = A->parent;
    if (B == nullptr or C == nullptr) return;

    //step1: left
    if (C->right != nullptr) C->right->parent = A;
    A->left = C->right;
    C->parent = A;

    if (C->left != nullptr) C->left->parent = B;
    B->right = C->left;

    B->parent = C;
    C->left = B;

    //step2: right
    C->parent = A->parent;
    A->parent = C;
    C->right = A;

    // handle root tag
    if(A->is_root) {
        _root->right = C;
        A->is_root = false;
        C->is_root = true;
    }

    // update head node in parent
    if (insert.position == 0) insert.node->left = C;
    else if (insert.position == 1) insert.node->right = C;

    // process height for A,B
    A->processHeight();
    B->processHeight();

    // start checkHeight and balance process
    C->checkHeight();
    C->processBalance();
}

void Node::RRRotation(NodePosition insert) {

/* Process Left rotation, this append when inserting node F:
 * - noted connexions are replaced by + noted connexions
 *         A            >          B
 *        / \-          >        +/ \
 *       E   B          >        A   C
 *         -/ \         >       / \+  \
 *         D   C        >      E   D   F
 *              \       >
 *               F      >
 * @param insert This define the parent of the subtree root node.
*/

    // assign A,B,D nodes
    Node* A = this;
    Node* B;
    Node* D;
    Node* _root = A->parent;
    B = this->right;
    D = B->left;
    if (B == nullptr) return;

    /* --------- connect B <=> parent --------- */
    B->parent = A->parent;

    // keep the root tag if rotate root
    if(A->is_root){
        _root->right = B;
        A->is_root = false;
        B->is_root = true;
    }

    // set B as parent child instead of A
    if (insert.position == 0) insert.node->left = B;
    else if (insert.position == 1) insert.node->right = B;

    /* --------- connect A <=> D --------- */
    A->right = D;
    if (D != nullptr)D->parent = A;

    /* --------- connect A <=> B --------- */
    B->left = A ;
    A->parent = B;

    // Check weight & balance
    A->checkHeight();
    A->processBalance();
}

void Node::remove() {
    /*
     * Trigger remove process for child and delete node pointer
     */
    if (this->left != nullptr) this->left->remove();
    if (this -> right != nullptr) this -> right -> remove();
    delete this;

}

// AVLTree methods

AVLTree::AVLTree() {
    uint256_t a("0");
    this->tree_root = new Node{a, 0, -1, nullptr, nullptr, nullptr,
                               0, nullptr, false, true};

    this->history = std::list<uint256_t>();

}

AVLTree::~AVLTree() {

    this->tree_root->right->remove();
    delete this->tree_root;
}

Node* AVLTree::insert(uint256_t key, uint64_t value) {
    /*
     * Insert a Node into the AVLTree
     *
     * @param key
     * @param value
     *
     * @return the Node* which have been inserted, nullptr if key already exist.
     */

    Node* node = new Node{key, value, -1, nullptr, nullptr, nullptr,
                          0, this->tree_root, false, false};

    // if tree is empty
    if (this->tree_root->right == nullptr) {
        this->setRoot(node);
        node->checkHeight();
        return node;
    }

    else{
        // Check for inserting position
        NodePosition position = this->insert_position(key);

        // if key already exists
        if (position.node == nullptr){
            delete node;
            return nullptr;
        }

        // insert as left child
        else if (position.position == 0){
            position.node->left = node;
            node->parent = position.node;
        }

        // insert as right child
        else if (position.position == 1){
            position.node->right = node;
            node->parent = position.node;
        }

        else {
            std::cout << "AVLTree::insert() => wrong insert position!";
            delete node;
            return nullptr;
        }

        // trigger height and balance check
        node->checkHeight();
        node->processBalance();

        return node;
    }
}

void AVLTree::setRoot(Node* node) {
    /*
     * Set the node as root node
     *
     * @param node the Node* to set as root node
     */

    this->tree_root->right = node;
    this->tree_root->right = node;
    node->is_root = true;
    node->parent = this->tree_root;



}

NodePosition AVLTree::insert_position(uint256_t key) {
    /*
     * Check the position where to insert the node
     *
     * @param key the key to inser at
     *
     * @return parent and position to insert in, nullptr if no possible position
     */

    // Start by root node
    Node* next = this->tree_root->right;
    NodePosition output = {nullptr, -1};

    while (next != nullptr){

        // if key already exists
        if (next->key == key){
            return output;
        }

        // if key lower than root key go left
        else if (key < next->key){

            // if no left child insert there
            if (next->left == nullptr){
                output.node = next;
                output.position = 0;
                return output;
            }

            // go through left child
            else next = next->left;
        }

        // if key higher than root key go right
        else if (next->key < key){

            // if no right child insert there
            if (next->right == nullptr){
                output.node = next;
                output.position = 1;
                return output;
            }

            // go through right child
            else next = next->right;
        }
    }

    return output;
}

Node *AVLTree::search(uint256_t key) {
    /*
     * Search if key exist in AVLTree
     *
     * @param key the key to look at
     *
     * @return Node* stored at key adress, nullptr if not found.
     */

    // start at root node
    Node* next = this->tree_root->right;
    Node* output;

    while (next != nullptr){
        // if key already exists
        if (next->key == key){
            output =  next;
            next = nullptr;
            break;
        }

        // if key lower than node key go left
        else if (key < next->key){
            // the key is not in the tree
            if (next->left == nullptr){
                output = nullptr;
                break;
            }

            // go through left child
            else next = next->left;
        }

        // if key higher than node key go right
        else if (next->key < key){
            // the key is not in the tree
            if (next->right == nullptr){
                output = nullptr;
                break;
            }

            // go through right child
            else next = next->right;
        }
    }

    return output;
    }

std::uint64_t AVLTree::getHeight() {
    /*
     * @ return the tree hight
     */
    return this->tree_root->height;
}
//
//uint256_t AVLTree::random_256() {
//    /*
//     * Generate a random uint256_t, only use for testing purposes.
//     */
//
//    std::random_device rd;
//    std::mt19937_64 gen(rd());
//
////    std::uniform_int_distribution<std::uint64_t> dist(0, 10000);
////    uint256_t out = {0, 0, 0, dist(gen)};
//
//    std::uniform_int_distribution<std::uint64_t> dist;
//    uint64_t a = dist(gen);
//    uint64_t b = dist(gen);
//    uint64_t c = dist(gen);
//    uint64_t d = dist(gen);
//    uint256_t out = {a,b,c,d};
//
//    return out;
//
//}

//void test_avl_tree() {
//    std::cout << "START MAIN() ---------------------------" << std::endl;
//    AVLTree tree = AVLTree();
//
//    /* ----------------------------Inserting keys --------------------------------------*/
//    std::cout << "Start inserting keys" << std::endl;
//    auto start = std::chrono::steady_clock::now();
//    int i = 0;
//    int colision = 0;
//    uint256_t k{};
//    while (i < 1000000){
//        k= tree.random_256();
//        if (tree.insert(k, 0) != nullptr) { i++; tree.history.push_back(k);}
//        else colision++;
//
//        if (i%100000 == 0) std::cout << i << std::endl;
//    }
//
//
//
//
//    auto duration = std::chrono::steady_clock::now() - start;
//    std::cout << "Total duration " << std::chrono::duration_cast<std::chrono::duration<double>>(duration) ;
//    std::cout << " for insert " << tree.history.size() << " keys" << std::endl;
//
//    std::cout << "Tree height =" << tree.getHeight() << std::endl;
//
//    /* ------------------------------------- searching keys ------------------------------------- */
//    Node* ret;
//    int found = 0;
//    int lost = 0;
//    i = 0;
//
//    std::cout << "Start retrieving keys ......." << std::endl;
//    start = std::chrono::steady_clock::now();
//    for(uint256_t element : tree.history) {
////        std::cout << "search for " << element.to_hex() << std::endl;
//        ret = tree.search(element);
//        if (ret == nullptr) {lost++;std::cout << "lost" << std::endl;}
//        else found++;
//        i++;
//
//        if (i%100000 == 0) std::cout << "Total=" << i << " / Found=" << found << " / Lost="
//                                     << lost <<std::endl;
//    }
//
//    duration = std::chrono::steady_clock::now() - start;
//    std::cout << "Total duration " << std::chrono::duration_cast<std::chrono::duration<double>>(duration) ;
//    std::cout << " for search " << tree.history.size() << " keys" << std::endl;
//    std::cout << "Total=" << i << " / Found=" << found << " / Lost=" << lost <<std::endl;
//    std::cout << "Colisions: " << colision << std::endl;
//
//}


