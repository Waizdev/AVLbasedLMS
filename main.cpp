#include <iostream>
#include <string>
using namespace std;

class Book {
public:
    int ISBN;
    string title;
    string author;
    
    Book(int i, string t, string a) 
	{
	  ISBN = i;
	  title = t;
	  author =a ; 
}
};

class Node {
public:
    Book book;
    Node* left;
    Node* right;
    int height;

    Node(int i, string t, string a)
        : book(i, t, a), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;

    int getHeight(Node* node) {
        if(node!=nullptr) {
		return node->height;
	}
	return	 0;
    }

    int getBalance(Node* node) {
        if(node!=nullptr){
		return  getHeight(node->left) - getHeight(node->right);
		} 
		return 0;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        return y;
    }

    Node* insert(Node* node, int ISBN, string title, string author) {
        if (!node) 
		return new Node(ISBN, title, author);
        if (ISBN < node->book.ISBN)
            node->left = insert(node->left, ISBN, title, author);
        else if (ISBN > node->book.ISBN)
            node->right = insert(node->right, ISBN, title, author);
        else
            return node;

        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
        int balance = getBalance(node);

        if (balance > 1 && ISBN < node->left->book.ISBN)
            return rotateRight(node);
        if (balance < -1 && ISBN > node->right->book.ISBN)
            return rotateLeft(node);
        if (balance > 1 && ISBN > node->left->book.ISBN) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && ISBN < node->right->book.ISBN) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* minValueNode(Node* node) {
        while (node && node->left)
            node = node->left;
        return node;
    }

    Node* deleteNode(Node* root, int ISBN) {
        if (!root) return root;
        if (ISBN < root->book.ISBN)
            root->left = deleteNode(root->left, ISBN);
        else if (ISBN > root->book.ISBN)
            root->right = deleteNode(root->right, ISBN);
        else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) {
                    delete root;
                    return nullptr;
                } else {
                    *root = *temp;
                    delete temp;
                }
            } else {
                Node* temp = minValueNode(root->right);
                root->book = temp->book;
                root->right = deleteNode(root->right, temp->book.ISBN);
            }
        }

        if (!root) return root;
        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    void inorder(Node* root) {
        if (root) {
            inorder(root->left);
            cout << "ISBN: " << root->book.ISBN ;
			cout<< "\nTitle: " << root->book.title; 
			cout<< "\nAuthor: " << root->book.author << "\n";
            inorder(root->right);
        }
    }

    Node* search(Node* root, int ISBN) {
        if (!root || root->book.ISBN == ISBN)
            return root;

        if (ISBN < root->book.ISBN)
            return search(root->left, ISBN);
        return search(root->right, ISBN);
    }

    void displayRange(Node* root, int minISBN, int maxISBN) {
        if (!root) return;
        if (root->book.ISBN > minISBN)
            displayRange(root->left, minISBN, maxISBN);
        if (root->book.ISBN >= minISBN && root->book.ISBN <= maxISBN)
            cout << "ISBN: " << root->book.ISBN;
			cout << "\nTitle: " << root->book.title;
			cout << "\nAuthor: " << root->book.author << "\n";
        if (root->book.ISBN < maxISBN)
            displayRange(root->right, minISBN, maxISBN);
    }

public:
    AVLTree() {
    	root = nullptr;
	}

    void insertBook(int ISBN, string title, string author) {
        root = insert(root, ISBN, title, author);
    }

    void deleteBook(int ISBN) {
        root = deleteNode(root, ISBN);
    }

    void displayBooks() {
        inorder(root);
    }

    void searchBook(int ISBN) {
        Node* result = search(root, ISBN);
        if (result) {
            cout << "Found Book: \nISBN: " << result->book.ISBN;
			cout<< "\nTitle: " << result->book.title;
			cout<< "\nAuthor: " << result->book.author << "\n";
        } else {
            cout << "Book with ISBN " << ISBN << " not found.\n";
        }
    }

    void displayBooksInRange(int minISBN, int maxISBN) {
        displayRange(root, minISBN, maxISBN);
    }

    void deleteTree() {
        while (root)
            root = deleteNode(root, root->book.ISBN);
    }
};

int main() {
    AVLTree tree;
    int choice, ISBN, minISBN, maxISBN;
    string title, author;
cout <<"  welcome to book store management system\n";
    while (true) {
        cout << "  ____________________________________\n";
        cout << " | Make your choice in numbers please |\n";
        cout << " |____________________________________|\n";
        cout << " | 1. Add a book                      |\n";
        cout << " |____________________________________|\n";
		cout << " | 2. Delete a book                   |\n";
        cout << " |____________________________________|\n";
		cout << " | 3. Search for a book               |\n";
        cout << " |____________________________________|\n";
		cout << " | 4. Display all books               |\n";
        cout << " |____________________________________|\n";
		cout << " | 5. Display books in a range        |\n";
        cout << " |____________________________________|\n";
		cout << " | 6. Exit                            |\n";
        cout << " |____________________________________|\n";
        cout << "Enter your choice: ";
        cin>>choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n'); 
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                cout << "Enter ISBN: ";
                cin >> ISBN;
                cin.ignore(); 
                cout << "Enter Title: ";
                getline(cin, title);
                cout << "Enter Author: ";
                getline(cin, author);
                tree.insertBook(ISBN, title, author);
                break;
            case 2:
                cout << "Enter ISBN to delete: ";
                cin >> ISBN;
                tree.deleteBook(ISBN);
                break;
            case 3:
                tree.displayBooks();
                break;
            case 4:
                cout << "Enter ISBN to search: ";
                cin >> ISBN;
                tree.searchBook(ISBN);
                break;
            case 5:
                cout << "Enter minimum ISBN: ";
                cin >> minISBN;
                cout << "Enter maximum ISBN: ";
                cin >> maxISBN;
                tree.displayBooksInRange(minISBN, maxISBN);
                break;
            case 6:
            	cout <<"Exiting system.......";
                tree.deleteTree();
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }
}

