#ifndef SET_HPP
#define SET_HPP

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#include "Node.h"
#include "Random.hpp"

template<typename T>
using index_set = __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<T>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;

class NodeContainer : private index_set<Node> {
public:
    NodeContainer() : sz(0) {}

    void insert(const Node& v) {
        auto res = index_set<Node>::insert(v);
        if (res.second) ++sz;
    }

    void erase(const Node& v) {
        auto res = index_set<Node>::erase(v);
        if (res) --sz;
    }

    inline uint size() const {
        return sz;
    }

    bool find(const Node& v) const {
        return index_set<Node>::find(v) != index_set<Node>::end();
    }

    Nodes randomChoose(uint k) {
        //std::cout << "nd ctn choose " << k << '\n';
        auto ch = Random::choose(size(), k);
        Nodes re;
        for (auto& v : ch) {
            re.push_back(*index_set<Node>::find_by_order(v));
        }
        return re;
    }

protected:
    uint sz;
//     struct Node {
//         Node() : leftchild(0), rightchild(0), parent(0), key(0), color(0), size(1) {}
//         Node(const Entry& key) : leftchild(0), rightchild(0), parent(0), key(key), color(0), size(0) {}

//         Node *leftchild = nullptr;
//         Node *rightchild = nullptr;
//         Node *parent = nullptr;
//         Entry key;
//         uint size;
//         bool color;             // for color: 0: Red, 1: Black
//         friend class Set;
        
//         Entry GetKey() const { return key; }
//     };

//     typename Set<Entry>::Node *root;

//     void LeftRotation(Node *x) {
//         std::cout << "left rot" << std::endl << '\n';
//         Node *y = x->rightchild;         // 把y指向x的rightchild, 最後y會變成x的parent

//         x->rightchild = y->leftchild;        // 圖二(c)左, 把y的leftchild托在x的rightchild

//         if (y->leftchild != nullptr){           // 圖二(c)右, 若node(j)為NIL則忽略
//             y->leftchild->parent = x;        // 將原先y的leftchild的parent改成x
//         }

//         y->parent = x->parent;               // 圖二(d)左, 更新y的parent

//         if (x->parent == nullptr){              // 圖二(d)右, 若原先x是root, 旋轉後y變成新的root
//             root = y;          
//         }         
//         else if (x == x->parent->leftchild){ // 若原先x是其parent的leftchild
//             x->parent->leftchild = y;        // 更新後y也是其parent的leftchild
//         }
//         else{                                // 若原先x是其parent的rightchild
//             x->parent->rightchild = y;       // 更新後y也是其parent的rightchild
//         }
//         y->leftchild = x;                    // 圖二(e)左, 最後才修改y與x
//         x->parent = y;                       // 圖二(e)右
//     }
//     void RightRotation(Node *y) {
//         std::cout << "right rot" << std::endl;
//         Node *x = y->leftchild;           // 把x設成y的leftchild

//         y->leftchild = x->rightchild;         // 把x的rightchild放到y的leftchild    
//         if (x->rightchild != nullptr){           // 若x的rightchild不為NIL, 將其parent指向y
//             x->rightchild->parent = y;
//         }
//         x->parent = y->parent;                // 將x的parent指向原先y的parent
//                                             // 以下一組if-else將修改原先y的parent之child
//         if (y->parent == nullptr){               // 若y原先是root, x將成為新的root
//             root = x;          
//         }              
//         else if (y == y->parent->leftchild){  // 若原先y是其parent之leftchild, 
//             y->parent->leftchild = x;         // x就成為其新的parent之leftchild
//         }
//         else{                                 // 若原先y是其parent之rightchild, 
//             y->parent->rightchild = x;        // x就成為其新的parent之rightchild
//         }
//         x->rightchild = y;                    // 將y設為x之rightchild
//         y->parent = x;                        // 將x設為y之parent
//     }
//     void InsertFixedUpRBT(Node *x) {
//         std::cout << "insert fix" << std::endl;
//         // case0: parent是黑色, 就不用進回圈
//         while (x != root && x->parent->color == 0) {   // 若parent是紅色即進入迴圈

//             // 上半部：parent是grandparent的left child
//             if (x->parent == x->parent->parent->leftchild) { 
//                 Node *y = x->parent->parent->rightchild;
//                 // case1: 若uncle是紅色
//                 if (y->color == 0) {
//                     x->parent->color = 1;
//                     y->color = 1;
//                     x->parent->parent->color = 0;              //grandparent改成紅色
//                     x = x->parent->parent;
//                 }
//                 // case2 & 3: uncle是黑色
//                 else {  
//                     if (x == x->parent->rightchild){     // case2
//                         x = x->parent;
//                         LeftRotation(x);
//                     }
//                     // case3
//                     x->parent->color = 1;                      //把parent塗黑
//                     x->parent->parent->color = 0;              // grandparent塗紅
//                     RightRotation(x->parent->parent);
//                 }
//             }
//             // 下半部：parent是grandparent的right child, 與上半部對稱
//             else {  
//                 Node *y = x->parent->parent->leftchild;
//                 // case1: 若uncle是紅色
//                 if (y->color == 0) {
//                     x->parent->color = 1;
//                     y->color = 1;
//                     x->parent->parent->color = 0;              //grandparent改成紅色
//                     x = x->parent->parent;
//                 }
//                 // case2 & 3: uncle是黑色
//                 else {
//                     if (x == x->parent->leftchild) {     // case2
//                         x = x->parent;
//                         RightRotation(x);
//                     }
//                     // case3
//                     x->parent->color = 1;
//                     x->parent->parent->color = 0;
//                     LeftRotation(x->parent->parent);
//                 }
//             }
//         }
//         root->color = 1;    // 確保root是黑色
//     }
//     void DeleteFixedUpRBT(Node *current) {
//         std::cout << "erase fix" << std::endl;
//         // Case0:(i)  current是紅色的, 直接把它塗黑
//         //       (ii) current是root, 直接把它塗黑
//         while (current != root && current->color == 1) {
//             // current是leftchild
//             if (current == current->parent->leftchild) {    
//                 Node *sibling = current->parent->rightchild;
//                 // Case1: 如果sibling是紅色
//                 if (sibling->color == 0) {
//                     sibling->color = 1;
//                     current->parent->color = 0;
//                     LeftRotation(current->parent);
//                     sibling = current->parent->rightchild;
//                 }
//                 // 進入 Case2、3、4: sibling是黑色
//                 // Case2: sibling的兩個child都是黑色
//                 if (sibling->leftchild->color == 1 && sibling->rightchild->color == 1) {
//                     sibling->color = 0;
//                     current = current->parent;           // 若current更新到root, 即跳出迴圈
//                 }
//                 // Case3 & 4: current只有其中一個child是黑色
//                 else {
//                     // case3: sibling的right child是黑的, left child是紅色
//                     if (sibling->rightchild->color == 1){
//                         sibling->leftchild->color = 1;
//                         sibling->color = 0;
//                         RightRotation(sibling);
//                         sibling = current->parent->rightchild;
//                     }
//                     // 經過Case3後, 一定會變成Case4
//                     // Case 4: sibling的right child 是紅色的, left child是黑色
//                     sibling->color = current->parent->color;
//                     current->parent->color = 1;
//                     sibling->rightchild->color = 1;
//                     LeftRotation(current->parent);
//                     current = root;     // 將current移動到root, 一定跳出迴圈
//                 }
//             }
//             // current是rightchild
//             else {  
//                 Node *sibling = current->parent->leftchild;
//                 // Case1: 如果sibling是紅色
//                 if (sibling->color == 0) {
//                     sibling->color = 1;
//                     current->parent->color = 0;
//                     RightRotation(current->parent);
//                     sibling = current->parent->leftchild;
//                 }
//                 // 進入 Case2、3、4: sibling是黑色
//                 // Case2: sibling的兩個child都是黑色
//                 if (sibling->leftchild->color == 1 && sibling->rightchild->color == 1) {
//                     sibling->color = 0;
//                     current = current->parent;             // 若current更新到root, 即跳出迴圈
//                 }
//                 // Case3 & 4: current只有其中一個child是黑色
//                 else {
//                     // case3: sibling的left child是黑的, right child是紅色
//                     if (sibling->leftchild->color == 1){
//                         sibling->rightchild->color = 1;
//                         sibling->color = 0;
//                         LeftRotation(sibling);
//                         sibling = current->parent->leftchild;
//                     }
//                     // 經過Case3後, 一定會變成Case4
//                     // Case 4: sibling的left child 是紅色的, rightt child是黑色
//                     sibling->color = current->parent->color;
//                     current->parent->color = 1;
//                     sibling->leftchild->color = 1;
//                     RightRotation(current->parent);
//                     current = root;     // 將current移動到root, 一定跳出迴圈
//                 }
//             }
//         }
//         current->color = 1;

//     }
//     Node* Successor(Node *current) {       // called by DeleteRBT()
//         std::cout << "succ" << std::endl;
//         if (current->rightchild != nullptr){
//             return Leftmost(current->rightchild);
//         }
        
//         Node *new_node = current->parent;
        
//         while (new_node != nullptr && current == new_node->rightchild) {
//             current = new_node;
//             new_node = new_node->parent;
//         }
//         std::cout << "end succ" << std::endl;
//         return new_node;
//     }

//     Node* Leftmost(Node *current) {      // called by Successor() 
//         std::cout << "l most" << std::endl;
//         while (current->leftchild != nullptr){
//             current = current->leftchild;
//         }
//         std::cout << "end l most" << std::endl;
//         return current;
//     }
//     Node* Search(const Entry& KEY) const {   // called by DeleteRBT() 
//         std::cout << "search" << std::endl;
//         Node *current = root;               // 將curent指向root作為traversal起點
            
//         while (current != nullptr && KEY != current->key) {  // 兩種情況跳出迴圈：
//                                                         // 1.沒找到 2.有找到
//             if (KEY < current->key){                      
//                 current = current->leftchild;   // 向左走
//             }
//             else {
//                 current = current->rightchild;  // 向右走
//             }
//         }
//         std::cout << "end search" << std::endl;
//         return current;
//     }

//     void erase(Node* delete_node) {

//         Node *y = 0;     // 真正要被刪除並釋放記憶體的node
//         Node *x = 0;     // 要被刪除的node的"child"

//         if (delete_node->leftchild == nullptr || delete_node->rightchild == nullptr){
//             y = delete_node;
//         }
//         else{
//             y = Successor(delete_node);         // 將y設成delete_node的Successor
//         }                                       // 經過這組if-else, y調整成至多只有一個child
        

//         if (y->leftchild != nullptr){              // 將x設成y的child, 可能有實際資料, 也有可能是NIL
//             x = y->leftchild;
//         }
//         else{
//             x = y->rightchild;
//         }

//         x->parent = y->parent;                 // 即使x是NIL也要把x的parent指向有效的記憶體位置
//                                             // 因為在FixUp時需要藉由x->parent判斷x為leftchild或是rightchild

//         if (y->parent == nullptr){                // 接著再把要被釋放記憶體的node之"parent"指向新的child
//             this->root = x;                    // 若刪除的是原先的root, 就把x當成新的root 
//         }
//         else if (y == y->parent->leftchild){   // 若y原本是其parent之left child
//             y->parent->leftchild = x;          // 便把x皆在y的parent的left child, 取代y
//         }
//         else{                                  // 若y原本是其parent之right child
//             y->parent->rightchild = x;         // 便把x皆在y的parent的right child, 取代y
//         }

//         if (y != delete_node) {                // 針對case3
//             delete_node->key = y->key;         // 若y是delete_node的替身, 最後要再將y的資料]
//         }

//         if (y->color == 1) {                   // 若刪除的node是黑色, 要從x進行修正, 以符合RBT的顏色規則
//             DeleteFixedUpRBT(x);
//         }
//     }

//     void dfs(Node* cur) {
//         std::cout << '(';
//         if (cur->leftchild != NULL) {
//             dfs(cur->leftchild);
//         }
//         std::cout << cur->key;
//         if (cur->rightchild != NULL) {
//             dfs(cur->rightchild);
//         }
//         std::cout << ')';
//     }
// public:
//     Set() {
//         root = nullptr;
//     };
    
//     Entry randomExtract() { 
//         Node* cur = root;
//         while (Random::trail(1 - 1 / (double)cur->size)) { // miss choosing tmp
//             if (cur->leftchild != NULL && Random::trail(cur->leftchild->size / (double)(cur->size - 1))) {
//                 cur = cur->leftchild;
//             }
//             else {
//                 cur = cur->rightchild;
//             }
//         }
        
//         Entry re = cur->key;
//         erase(cur);
//         return re;
//      }
//     bool find(const Entry& key) const { return Search(key) != nullptr; }
//     void insert(const Entry& key) {
//         std::cout << "insert" << std::endl;
//         // 前半部與 InsertBST()之邏輯完全相同, 僅僅需要修改 NULL <-> NIL
//         Node *y = root;
//         Node* tmp = root;
//         Node *x = new Node(key);

//         while (tmp != nullptr) {     // 把root初始化成neel, 這裡就可以用neel來做判斷
//             y = tmp;
//             if (x->key < tmp->key){
//                 tmp = tmp->leftchild;
//             }
//             else if (x->key > tmp->key){
//                 tmp = tmp->rightchild;
//             }
//             else {
//                 return; // already exists
//             }
//         }

//         x->parent = y;
//         if (y == nullptr) {
//             root = x;
//         }
//         else if (y->key < x->key) {
//             y->rightchild = x;
//         }
//         else {
//             y->leftchild = x;
//         }

//         // while (x != root && x->parent->color == 0) {
//         //     if (x->parent == x->parent->parent->leftchild) {
//         //         y = x->parent->parent->rightchild;
//         //         if (y->color == 0) {
//         //             x->parent->color = 1;
//         //             y->color = 1;
//         //             x->parent->parent->color = 0;
//         //             x = x->parent->parent;
//         //         }
//         //         else if (x == x->parent->rightchild) {
//         //             x = x->parent;
//         //             LeftRotation(x);

//         //         }
//         //     }
//         // }
            
//         InsertFixedUpRBT(x);      // 對可能出現紅色與紅色node相連之情形做修正
//         dfs(root);
//         std::cout << std::endl;
//         std::cout << "end insert" << std::endl;
//     }
//     void erase(const Entry& KEY) {              // 要刪除具有KEY的node
//         std::cout << "erase" << std::endl;    
//         Node *delete_node = Search(KEY);   // 先確認RBT中是否存在具有KEY的node
//         if (delete_node == NULL) {
//             throw std::runtime_error("element not found");
//         }

//         erase(delete_node);
//         dfs(root);
//         std::cout << std::endl;
//         std::cout << "end erase" << std::endl;
//     }
};

#endif