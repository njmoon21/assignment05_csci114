#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct FreeListNode {
    int block_id_, free_memory_size_;
    FreeListNode* next;

    FreeListNode(int BlockIdNumber) : block_id_(BlockIdNumber), free_memory_size_(1024),next(nullptr) {}
};

class FreeList {
private:
    FreeListNode* head;
    FreeListNode* tail;

public:
    FreeList() : head(nullptr), tail(nullptr) {
        for (int i = 1; i <= 1024; i++)
            addNode(i);
    }
    ~FreeList() {
        FreeListNode* current = head;
        while (current) {
            FreeListNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }


    void addNode(int BlockId) { // Equivalent to push_back()
        FreeListNode* newNode = new FreeListNode(BlockId);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // void SortFreeList();
};

struct AllocationListNode {
    int request_id_, block_id_, memory_size_;
    AllocationListNode* next;

    AllocationListNode(int requestID, int blockID, int memorySize) :
        request_id_(requestID), block_id_(blockID), memory_size_(memorySize), next(nullptr) {}
};

class AllocationList {
private:
    AllocationListNode* head;
    AllocationListNode* tail;

public:
    AllocationList() : head(nullptr), tail(nullptr) {}
    ~AllocationList() {
        AllocationListNode* current = head;
        while (current) {
            AllocationListNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }


    // Adding node from the back
    void addNode(int requestID, int blockID, int memorySize) {
        AllocationListNode* newNode = new AllocationListNode(requestID, blockID, memorySize);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }
};

int main() {
    // Initializing free list
    FreeList* freeList = new FreeList();

    // For traversing through the free list


    std::ifstream requestFile("requests-1.txt");
    std::string inputString;
    int requestCounterA = 1;
    int requestCounterR = 1;

    while (std::getline(requestFile, inputString)) {


        // Parsing each line from request-1.txt, defining each variable
        std::string userRequestType;
        int userRequestID, userAllocationSize;

        std::istringstream iss(inputString);
        iss >> userRequestType >> userRequestID;


        // Determine next instruction based on request type
        if (userRequestType == "A") {
            iss >> userAllocationSize;
            std::cout << userAllocationSize << " bytes have been allocated at block " << "block #"
                      << " for request " << requestCounterA << std::endl;
            requestCounterA++;
        }
        else if (userRequestType == "R") {
            std::cout << "Released bytes for request " << requestCounterR << std::endl;
            requestCounterR++;
        }

        else
            std::cout << "Invalid request type\n";


    }
    requestFile.close();


    // Deleting all nodes in a linked list


    std::ofstream finalSizeFile("final_size.txt");



    finalSizeFile.close();
    delete freeList;

    return 0;
}