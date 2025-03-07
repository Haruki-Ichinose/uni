#include <stdio.h>
#include <stdlib.h>

// Binary search treeのノードを表す構造体
typedef struct TreeNode
{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Binary search treeを表す構造体
typedef struct BinarySearchTree
{
    TreeNode *root;
} BinarySearchTree;

// Binary search treeを初期化する関数
void initializeTree(BinarySearchTree *tree)
{
    tree->root = NULL;
}

// ノードを挿入する関数
TreeNode *insertNode(TreeNode *root, int data)
{
    if (root == NULL)
    {
        TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
        if (!newNode)
        {
            perror("メモリ割り当てエラー");
            exit(EXIT_FAILURE);
        }
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (data < root->data)
    {
        root->left = insertNode(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = insertNode(root->right, data);
    }

    return root;
}

// ノードを削除する関数
TreeNode *deleteNode(TreeNode *root, int data)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (data < root->data)
    {
        root->left = deleteNode(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = deleteNode(root->right, data);
    }
    else
    {
        if (root->left == NULL)
        {
            TreeNode *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            TreeNode *temp = root->left;
            free(root);
            return temp;
        }

        // 2つの子ノードがある場合
        TreeNode *minRightSubtree = root->right;
        while (minRightSubtree->left != NULL)
        {
            minRightSubtree = minRightSubtree->left;
        }

        root->data = minRightSubtree->data;
        root->right = deleteNode(root->right, minRightSubtree->data);
    }

    return root;
}

// ノードを検索する関数
TreeNode *searchNode(TreeNode *root, int data)
{
    if (root == NULL || root->data == data)
    {
        return root;
    }

    if (data < root->data)
    {
        return searchNode(root->left, data);
    }
    else
    {
        return searchNode(root->right, data);
    }
}

// 二分探索木を中間順（in-order）で表示する関数
void inOrderTraversal(TreeNode *root)
{
    if (root != NULL)
    {
        inOrderTraversal(root->left);
        printf("[%d] ", root->data);
        inOrderTraversal(root->right);
    }
}

// 二分探索木を前順（pre-order）で表示する関数
void preOrderTraversal(TreeNode *root)
{
    if (root != NULL)
    {
        printf("[%d] ", root->data);
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);
    }
}

int main()
{
    BinarySearchTree myTree;
    initializeTree(&myTree);

    // ノードを挿入
    myTree.root = insertNode(myTree.root, 20);
    insertNode(myTree.root, 10);
    insertNode(myTree.root, 30);
    insertNode(myTree.root, 5);
    insertNode(myTree.root, 15);

    // 二分探索木を中間順で表示
    printf("中間順（in-order）で表示: ");
    inOrderTraversal(myTree.root);
    printf("\n");

    // ノードを検索
    int searchData = 15;
    TreeNode *searchResult = searchNode(myTree.root, searchData);
    if (searchResult != NULL)
    {
        printf("データ %d が見つかりました\n", searchData);
    }
    else
    {
        printf("データ %d は見つかりませんでした\n", searchData);
    }

    // ノードを削除
    int deleteData = 20;
    myTree.root = deleteNode(myTree.root, deleteData);

    // 削除後の二分探索木を中間順で表示
    printf("削除後の中間順（in-order）で表示: ");
    inOrderTraversal(myTree.root);
    printf("\n");

    // 二分探索木を前順で表示
    printf("前順（pre-order）で表示: ");
    preOrderTraversal(myTree.root);
    printf("\n");

    return 0;
}
