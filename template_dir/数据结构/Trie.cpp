struct Node {
    int cnt = 0;
    std::array<Node*, 26> son{NULL};
};

Node* head = new Node();
auto insert = [&](string& x) {
    Node* cur = head;
    for (char ch : x) {
        int p = ch - 'a';
        if (cur -> son[p] != NULL) {
            cur = cur -> son[p];
            (cur -> cnt)++;
        } else {
            Node* nex = new Node();
            cur -> son[p] = nex;
            cur = nex;
            (cur -> cnt)++;
        }
    }
};
auto find = [&](string& x) {
    Node* cur = head;
    for (char ch : x) {
        int p = ch - 'a';
        if (cur -> son[p] != NULL) {
            cur = cur -> son[p];
        } else {
            return 0;
        }
    }
    return cur -> cnt;
};


// 0-1 Trie
const int MAX_BITS = 30;
struct Trie {
    struct Node {
        Node* children[2] = {nullptr, nullptr};
        int index = -1;
    };

    Node* root;

    Trie() {
        root = new Node();
    }

    void insert(int num, int idx) {
        Node* cur = root;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = num >> i & 1;
            if (!cur->children[bit]) {
                cur->children[bit] = new Node();
            }
            cur = cur->children[bit];
        }
        cur->index = idx;
    }

    // Max(Min) := 1(0)
    int query(int num, int tag){
        Node* cur = root;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = num >> i & 1;
            if(cur->children[bit ^ tag]) cur = cur->children[bit ^ tag];
            else cur = cur->children[bit ^ 1 ^ tag];
        }
        return cur->index;
    }

    pair<int, int> queryMax(int num) {
        Node* cur = root;
        int result = 0;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (cur->children[1 - bit]) {
                result |= (1 << i);
                cur = cur->children[1 - bit];
            } else {
                cur = cur->children[bit];
            }
        }
        return {result, cur->index};
    }

    pair<int, int> queryMin(int num) {
        Node* cur = root;
        int result = 0;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (cur->children[bit]) {
                cur = cur->children[bit];
            } else {
                result |= (1 << i);
                cur = cur->children[1 - bit];
            }
        }
        return {result, cur->index};
    }

    ~Trie() {
        clear(root);
    }

private:
    void clear(Node* node) {
        if (!node) return;
        clear(node->children[0]);
        clear(node->children[1]);
        delete node;
    }
};


// Remove(ChatGPT)
const int MAX_BITS = 30; // 数字范围最大为 1e9，需支持最高位为 2^30。

struct Trie {
    struct Node {
        Node* children[2] = {nullptr, nullptr}; // 0 和 1 两个分支
        int count = 0; // 当前路径上数字的计数
        int index = -1; // 记录当前路径数字的索引（数组下标），-1 表示未终止。
    };

    Node* root;

    Trie() {
        root = new Node();
    }

    // 插入一个数字及其对应索引
    void insert(int num, int idx) {
        Node* current = root;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = (num >> i) & 1; // 提取当前位
            if (!current->children[bit]) {
                current->children[bit] = new Node();
            }
            current = current->children[bit];
            current->count++; // 路径上数字计数+1
        }
        current->index = idx; // 叶子节点记录数字对应的索引
    }

    // 删除一个数字
    void remove(int num) {
        Node* current = root;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (current->children[bit]) {
                current = current->children[bit];
                current->count--; // 路径上数字计数-1
            }
        }
    }

    // 查询与 num 异或的最大值及其索引
    pair<int, int> queryMax(int num) {
        Node* current = root;
        int result = 0;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = (num >> i) & 1;
            // 尽量选择相反的位以最大化异或值
            if (current->children[1 - bit] && current->children[1 - bit]->count > 0) {
                result |= (1 << i); // 设置该位为 1
                current = current->children[1 - bit];
            } else {
                current = current->children[bit];
            }
        }
        return {result, current->index}; // 返回异或值和对应索引
    }

    // 查询与 num 异或的最小值及其索引
    pair<int, int> queryMin(int num) {
        Node* current = root;
        int result = 0;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = (num >> i) & 1;
            // 尽量选择相同的位以最小化异或值
            if (current->children[bit] && current->children[bit]->count > 0) {
                current = current->children[bit];
            } else {
                result |= (1 << i); // 设置该位为 1
                current = current->children[1 - bit];
            }
        }
        return {result, current->index}; // 返回异或值和对应索引
    }

    ~Trie() {
        clear(root);
    }

private:
    void clear(Node* node) {
        if (!node) return;
        clear(node->children[0]);
        clear(node->children[1]);
        delete node;
    }
};