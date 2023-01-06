#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

/*
* ################### Information #################
* Define Nodes
* Dynamic List
* Dynamic Types
* its more power full than array but array is the fastest way
* easy insert , remove nodes from list
* Traversing 
* Searching
* Sorting
* Merging
*/

#include <stdlib.h>

namespace DataStructures
{

#pragma region Declearation

	template <typename T>
	class Node
	{
	public:

		T Value;

		Node<T>* PreviousNodePtr;
		Node<T>* NextNodePtr;
	};


	template <typename T>
	class LinkedList
	{
	private:

		//Pointer For the First Node inside LinkedList
		Node<T>* _FirstNode;

		//Pointer For the Last Node inside LinkedList
		Node<T>* _LastNode;

		//Hold Nodes Count inside LinkedList
		int _Count;


	public:

		//Constructor
		LinkedList();

		//Destrcutor
		~LinkedList();


		//Insert New Node inside LinkedList
		void Insert(T data);

		//Insert New Node at Begin of LinkedList
		void InsertBegin(T data);

		//Delete Node inside LinkedList
		void Delete(T data);

		//Delete Node At Begin of LinkedList
		void DeleteBegin();

		//Print All Nodes
		void Display();

		//Print All Nodes Reversed From Bottom To Top
		void DisplayReverse();

		//Clear All Items Inside LinkedList
		void Clear();


		//Get the Count of Nodes inside LinkedList
		int getCount();

		//Get the First Node inside LinkedList
		Node<T>* FirstNode();

		//Get the Last Node inside LinkedList
		Node<T>* LastNode();

		//Check if LinkedList is Empty
		bool isEmpty();
		
		//check if data exist inside any node
		bool isExist(T data);

	};

#pragma endregion

#pragma region Defination

	//[Tested]
	//Constructor
	template <typename T>
	LinkedList<T>::LinkedList()
	{
		_FirstNode = nullptr;
		_LastNode = nullptr;

		_Count = 0;
	}

	//Desctructor
	template <typename T>
	LinkedList<T>::~LinkedList()
	{

	}


	//[Tested]
	//Get the Count of Nodes inside LinkedList
	template <typename T>
	int LinkedList<T>::getCount()
	{
		return _Count;
	}

	//[Tested]
	//Get the First Node inside LinkedList
	template <typename T>
	Node<T>* LinkedList<T>::FirstNode()
	{
		return _FirstNode;
	}

	//[Tested]
	//Get the Last Node inside LinkedList
	template <typename T>
	Node<T>* LinkedList<T>::LastNode()
	{
		return _LastNode;
	}

	//[Tested]
	//Insert New Node inside LinkedList
	template <typename T>
	void LinkedList<T>::Insert(T data)
	{
		//Increase the Counter of Nodes
		_Count++;

		//Create New Node inside Memory and Save its Address inside Pointer newNode
		//new will be used to make dynamic memory allocation inside Heap Section of memory
		//but pointer variable will be stored inside Stack Section of Memory and will be use pointer to access the array stored inside heap
		//programe cant access heap directlly only via pointer stored inside stack section
		//Node<T>* newNodePtr = new Node<T>();
		Node<T>* newNodePtr = (Node<T>*) malloc(sizeof(Node<T>));

		newNodePtr->Value = data;
		newNodePtr->NextNodePtr = nullptr;
		newNodePtr->PreviousNodePtr = _LastNode;


		//Link New Node to the Last Node inside Linked List

		if (_LastNode == nullptr)
		{
			//if the node is the first node inside LinkedList
			_FirstNode = newNodePtr;
			_LastNode = newNodePtr;
		}
		else
		{
			//if new node not the first node inside LinkedList

			//Update the NextNode inside the last Node
			_LastNode->NextNodePtr = newNodePtr;

			//Make Last Node Refere to the new last Node
			_LastNode = newNodePtr;
		}
		
	}

	//Insert New Node at Begin of LinkedList
	template <typename T>
	void LinkedList<T>::InsertBegin(T data)
	{
		//Create New Node inside Memory and Save its Address inside Pointer newNode
		//Node<T>* newNodePtr = new Node<T>();
		Node<T>* newNodePtr = (Node<T>*) malloc(sizeof(Node<T>));

		newNodePtr->Value = data;
		newNodePtr->NextNodePtr = _FirstNode;
		newNodePtr->PreviousNodePtr = nullptr;


		//Link New Node to the First Node inside Linked List

		if (isEmpty())
		{
			//if the node is the first node inside LinkedList
			_FirstNode = newNodePtr;
			_LastNode = newNodePtr;
		}
		else
		{
			//if new node not the first node inside LinkedList

			//Update the Previous inside the Old First Node
			_FirstNode->PreviousNodePtr = newNodePtr;

			//Make First Node Refere to the new last Node
			_FirstNode = newNodePtr;
		}

		//Increase the Counter of Nodes
		_Count++;
	}

	//Delete Node inside LinkedList
	template <typename T>
	void LinkedList<T>::Delete(T data)
	{
		//Search For data inside All Nodes
		Node<T>* currentNode = _FirstNode;
		Node<T>* previousNode = nullptr;
		Node<T>* nextNode = _FirstNode->NextNodePtr;

		//Loop All Nodes Until Found the Node
		while (currentNode->Value != data)
		{
			currentNode = currentNode->NextNodePtr;
			previousNode = currentNode->PreviousNodePtr;
			nextNode = currentNode->NextNodePtr;

			//When Reached to Last Node
			if (currentNode == nullptr)
			{
				break;
			}
		}

		//If not Found the Node to be Deleted
		if (currentNode == nullptr)
		{
			//std::cout << "Node Not Exist \n";
			return;
		}



		if (currentNode->Value == _FirstNode->Value)
		{
			//If Found Node is First Node
			if (nextNode != nullptr) {
				_FirstNode = nextNode;
				nextNode->PreviousNodePtr = nullptr;
			}
			
		}
		else if (currentNode->Value == _LastNode->Value)
		{
			//If Found Node is Last Node
			_LastNode = previousNode;
			previousNode->NextNodePtr = nullptr;
		}
		else
		{
			previousNode->NextNodePtr = nextNode;
			nextNode->PreviousNodePtr = previousNode;
		}

		_Count--;

		//Delete Current Node From Memory
		//delete currentNode;
		free(currentNode);
		//delete currentNode;

	}

	//Delete Node At Begin of LinkedList
	template <typename T>
	void LinkedList<T>::DeleteBegin()
	{
		Delete(_FirstNode->Value);
	}

	//[Tested]
	//Print All Nodes
	template <typename T>
	void LinkedList<T>::Display()
	{
		if (isEmpty())
		{
			//std::cout << "List is Empty";
		}
		else
		{
			Node<T>* currentNode = _FirstNode;

			//std::cout << "LinkedList Nodes" << std::endl;
			//std::cout << "================" << std::endl;

			while (currentNode != nullptr)
			{
				//std::cout << currentNode->Value << "\n";

				currentNode = currentNode->NextNodePtr;
			}

			//std::cout << "\n";
		}

	}

	//[Tested]
	//Print All Nodes Reversed From Bottom To Top
	template <typename T>
	void LinkedList<T>::DisplayReverse()
	{
		if (isEmpty())
		{
			//std::cout << "List is Empty";
		}
		else
		{
			Node<T>* currentNode = _LastNode;

			//std::cout << "LinkedList Nodes" << std::endl;
			//std::cout << "================" << std::endl;

			while (currentNode != nullptr)
			{
				//std::cout << currentNode->Value << "\n";

				currentNode = currentNode->PreviousNodePtr;
			}

			//std::cout << "\n";
		}

	}

	//[Tested]
	//Check if LinkedList is Empty
	template <typename T>
	bool LinkedList<T>::isEmpty()
	{
		return _Count == 0;
	}

	
	//check if data exist inside any node
	template <typename T>
	bool LinkedList<T>::isExist(T data)
	{
		bool exist = false;

		Node<T>* currentNode = _FirstNode;

		while (currentNode != nullptr)
		{
			if (currentNode->Value == data)
				return true;

			currentNode = currentNode->NextNodePtr;
		}

		return exist;
	}

	//Clear All Items Inside LinkedList
	template <typename T>
	void LinkedList<T>::Clear()
	{
		Node<T>* currentNode = _LastNode;
		Node<T>* previousNode;

		//Loop All Nodes Until Finish
		while (currentNode != nullptr)
		{
			previousNode = currentNode->PreviousNodePtr;

			free(currentNode);

			currentNode = previousNode;
		}

		_FirstNode = nullptr;
		_LastNode = nullptr;
		_Count = 0;
	}

#pragma endregion

}

#endif // !LINKEDLIST_H_


