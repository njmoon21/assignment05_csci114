#include <iostream>
#include <fstream>
#include <string>

struct Node {
    int block_id_;
    int free_memory_size_ = 1024; // Initializing all blocks with 1024 bytes (all same size -> ordered)
    Node* next;
};

struct AllocationNode {
    int request_id_;
    int block_id_;
    int memory_size_;
    AllocationNode* next;
};

// findSmallestBlock();
// updateFreeListOrder();

int main() {
    // Initializing free list
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int i = 1; i <= 1024; i++) {
        Node* newNode = new Node;

        newNode->block_id_ = i;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // For traversing through the free list
    Node* current = head;
    while (current != nullptr) {
        std::cout << "Block ID: " << current->block_id_ << ", Size: " << current->free_memory_size_ << std::endl;
        current = current->next;
    }

    std::ifstream requestFile("requests-1.txt");
    std::string inputString;

    while (std::getline(requestFile, inputString)) {
        int requestCounter = 1;

        // Parsing each line from request-1.txt, defining each variable
        std::string userRequestType = inputString.substr(0, 1);
        int userRequestId = std::stoi(inputString.substr(2, 1));
        int userAllocationSize = std::stoi(inputString.substr(4));


        // Determine next instruction based on request type
        if (userRequestType == "A")
            std::cout << userAllocationSize << " bytes have been allocated at block " << "block #"
                      << " for request " << requestCounter << std::endl;
        else if (userRequestType == "R")
            std::cout << "Release\n";
        else
            std::cout << "Invalid request type\n";

        requestCounter++;
    }
    requestFile.close();

    // Deleting all nodes in a linked list


    std::ofstream finalSizeFile("final_size.txt");



    finalSizeFile.close();
    return 0;
}