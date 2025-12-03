#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/*
 *  ----- HOW TO RUN THE PROGRAM -----
 *
 *  1. In Cygwin, compile program typing:
 *      g++ CSci113_P5.cpp
 *  (MAKE SURE to have 'requests-1.txt' in the same directory, or else the program won't work)
 *
 *  2. Type "./a.exe" to run the program
 *
 *  3. The output will show the blocks that have been allocated and freed. In the directory,
 *     there will be an output file called 'final-size.txt', where each number represents how much
 *     available bytes are in each memory block (1024 blocks in total)
 *
 *
 *     - NATHAN JACK LUNA
 */


struct FreeListNode {
    int block_id_, free_memory_size_;
    FreeListNode* next;

    FreeListNode(int BlockIdNumber) : block_id_(BlockIdNumber), free_memory_size_(1024), next(nullptr) {}
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


    FreeListNode* getHead() { return head; }


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


    FreeListNode* allocateFindNode(int userAllocationSize) {
        FreeListNode* current = head;

        while(current->free_memory_size_ < userAllocationSize && current->next != nullptr) {
            current = current->next;
        }
        if (current->next == nullptr && current->free_memory_size_ < userAllocationSize)
            return nullptr;
        return current;
    }


    FreeListNode* blockIDFindNode(int blockID) {
        FreeListNode* current = head;

        while(current->block_id_ != blockID && current->next != nullptr) {
            current = current->next;
        }
        if (current->next == nullptr)
            return nullptr;
        return current;
    }


    // Technically we aren't deleting the node, we are just removing and then inserting it back after
    void removeNode(FreeListNode* selectedNode) {
        if (head == selectedNode) {
            head = head->next;
        }
        else {
            FreeListNode* previous = head;

            while (previous->next != selectedNode)
                previous = previous->next;
            if (previous->next == selectedNode)
                previous->next = selectedNode->next;
        }
    }


    // Reinserting the node that we removed after we changed the value of the free_memory_size_
    void reinsertNode(FreeListNode* selectedNode) {
        if (head == nullptr || selectedNode->free_memory_size_ < head->free_memory_size_) {
            selectedNode->next = head;
            head = selectedNode;
            return;
        }
        else {
            FreeListNode* current = head;

            while (current->next != nullptr && current->next->free_memory_size_ <= selectedNode->free_memory_size_)
                current = current->next;
            selectedNode->next = current->next;
            current->next = selectedNode;
        }
    }
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


    // Deleting a node ; We don't need to call to sort the list, the allocation list is sorted by request ID
    void deleteNode(AllocationListNode* selectedNode) {
        if (head == selectedNode) {
            head = head->next;
            delete selectedNode;
        }
        else {
            AllocationListNode* previous = head;

            while (previous->next != selectedNode)
                previous = previous->next;
            if (previous->next == selectedNode)
                previous->next = selectedNode->next;
            delete selectedNode;
        }
    }


    AllocationListNode* releaseFindNode(int requestID) {
        AllocationListNode* current = head;

        while(current->request_id_ != requestID && current->next != nullptr) {
            current = current->next;
        }
        if (current->next == nullptr && current->request_id_ != requestID)
            return nullptr;
        return current;
    }
};


int main() {
    // Initializing both linked lists
    FreeList* freeList = new FreeList();
    AllocationList* allocationList = new AllocationList();


    std::ifstream requestFile("requests-1.txt");
    std::string inputString;
    int requestCounterA = 1;
    int requestCounterR = 1;


    while (std::getline(requestFile, inputString)) {
        // Parsing each line from request-1.txt, defining each variable
        std::string userRequestType;
        int userRequestID, userAllocationSize;

        /*
         *This is my first time using "istringstream" from the <sstream> library. I initially tried
         *using the substr method, however we are uncertain about the amount of chars the request #
         *and request size will take. I discovered that you can extract each section using the code below
         */
        std::istringstream iss(inputString);
        iss >> userRequestType >> userRequestID;

        // Determine next instruction based on request type
        if (userRequestType == "A") {
            iss >> userAllocationSize;

            FreeListNode* selectedNode = freeList->allocateFindNode(userAllocationSize);
            if (selectedNode == nullptr)
                std::cout << "Couldn't find available node.\n";
            else {
                std::cout << userAllocationSize << " bytes have been allocated at block " << selectedNode->block_id_
                          << " for request " << requestCounterA << std::endl;

                allocationList->addNode(userRequestID, selectedNode->block_id_, userAllocationSize);
                freeList->removeNode(selectedNode);
                selectedNode->free_memory_size_ = selectedNode->free_memory_size_ - userAllocationSize;
                freeList->reinsertNode(selectedNode);
            }

            requestCounterA++;
        }
        else if (userRequestType == "R") {
            AllocationListNode* selectedAllocatedNode = allocationList->releaseFindNode(userRequestID);
            if (selectedAllocatedNode == nullptr)
                std::cout << "Couldn't find available node.\n";
            else {
                FreeListNode* selectedFreeNode = freeList->blockIDFindNode(selectedAllocatedNode->block_id_);
                std::cout << selectedAllocatedNode->memory_size_ << " bytes have been returned back to block "
                          << selectedAllocatedNode->block_id_ << " for request " << selectedAllocatedNode->request_id_ << std::endl;

                freeList->removeNode(selectedFreeNode);
                selectedFreeNode->free_memory_size_ = selectedFreeNode->free_memory_size_ + selectedAllocatedNode->memory_size_;
                freeList->reinsertNode(selectedFreeNode);
                allocationList->deleteNode(selectedAllocatedNode);
            }

            requestCounterR++;
        }
        else
            std::cout << "Invalid request type\n";
    }
    requestFile.close();


    std::ofstream finalSizeFile("final_size.txt");
    FreeListNode* current = freeList->getHead();
    while (current != nullptr) {
        finalSizeFile << current->free_memory_size_ << std::endl;
        current = current->next;
    }
    finalSizeFile.close();


    delete freeList;
    delete allocationList;


    return 0;
}