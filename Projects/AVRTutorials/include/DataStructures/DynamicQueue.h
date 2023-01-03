#include "LinkedList.h"

#ifndef DynamicQueue_H_
#define DynamicQueue_H_

/*
* ################### Information #################
* Define Queue List Powered By LinkedList
* Dynamic List Items
* Enqueue item to End (Rear) of Queue
* Dequeue item from Front of Queue
* peak current Front item
* No Limits for items count
* Implement Rule of First-In First-Out
* Work Like Simple Queue but with no limits
*/

namespace DataStructures
{

#pragma region Decleration

	template <typename T>
	class DynamicQueue
	{
	private:

		//Linked List To Store Items of Queue
		LinkedList<T> _LinkedList;

		//Pointer For the Front (First) Item inside Queue
		Node<T>* _FrontPtr;
		//Pointer For the Rear (Last) Item inside Queue
		Node<T>* _RearPtr;

		//max items can be stored inside queue
		int _MaxCount;

		//if current queue is limited or not
		bool _LimitedQueue;

	public:

		//Constructor
		DynamicQueue();
		DynamicQueue(int maxCount);

		//Destrcutor
		~DynamicQueue();

		//Push Item inside End of Queue
		void Enqueue(T data);

		//Pop Item from start of Queue
		T Dequeue();

		//Get Front Item Value from Queue
		T Peak();

		Node<T>* FrontPtr();
		Node<T>* RearPtr();

		//Print All Items Order by the Dequeue sorting
		void Display();

		//Print All Items Order by the Enqueue sorting
		void DisplayReverse();

		//Clear All Items inside Queue
		void Clear();

		//Get the Top Item inside Queue
		T Front();

		//Get the Bottom Item inside Queue
		T Rear();

		//Get the Count of Items inside Queue
		int Count();

		//Check if Queue is Empty
		bool IsEmpty();

		//Check if Queue is Full only with limited queue
		bool IsFull();

		//check if data exist or not inside queue
		bool IsExist(T data);

	};

#pragma endregion

#pragma region Definations

	//Constructor
	template <typename T>
	DynamicQueue<T>::DynamicQueue()
	{
		_FrontPtr = nullptr;
		_RearPtr = nullptr;

		_MaxCount = -1; //unlimited queue
		_LimitedQueue = false;
	}

	template <typename T>
	DynamicQueue<T>::DynamicQueue(int maxCount)
	{
		_FrontPtr = nullptr;
		_RearPtr = nullptr;

		_MaxCount = maxCount; //limited queue
		_LimitedQueue = true;
	}


	//Desctructor
	template <typename T>
	DynamicQueue<T>::~DynamicQueue()
	{
		//Empty Arrays From Memory
		/*if (_Array != NULL)
		{
			free(_Array);
		}
		*/
	}




	//Get the Count of Items inside DynamicQueue
	template <typename T>
	int DynamicQueue<T>::Count()
	{
		return _LinkedList.getCount();
	}


	//Get the Top Item inside DynamicQueue
	template <typename T>
	T DynamicQueue<T>::Front()
	{
		if (_FrontPtr != nullptr) {
			return _FrontPtr->Value;
		}
		else {
			return nullptr;
		}

		
	}


	//Get the Bottom Item inside DynamicQueue
	template <typename T>
	T DynamicQueue<T>::Rear()
	{
		if (_RearPtr != nullptr) {
			return _RearPtr->Value;
		}
		else {
			return nullptr;
		}

	}

	template <typename T>
	Node<T>* DynamicQueue<T>::FrontPtr()
	{
		return _FrontPtr;
	}

	template <typename T>
	Node<T>* DynamicQueue<T>::RearPtr()
	{
		return _RearPtr;
	}

	//Check if Queue is Empty
	template <typename T>
	bool DynamicQueue<T>::IsEmpty()
	{
		return _LinkedList.isEmpty();
	}

	//Check if Queue is Full only with limited queue
	template <typename T>
	bool DynamicQueue<T>::IsFull()
	{
		return _LinkedList.getCount() == _MaxCount;
	}


	//check if data exist or not inside queue
	template <typename T>
	bool DynamicQueue<T>::IsExist(T data) 
	{
		return _LinkedList.isExist(data);
	}

	//Push Item inside Queue
	template <typename T>
	void DynamicQueue<T>::Enqueue(T data)
	{
		//check if queue is limited or not
		if (_LimitedQueue && IsFull())
		{
			//std::cout << "Queue is Limited Overflow";
			return;
		}

		//Store the data inside LinkedList
		_LinkedList.Insert(data);


		//Update Pointer of Top,Bottom to Refere to the Top Item , Bottom Item
		_FrontPtr = _LinkedList.FirstNode();
		_RearPtr = _LinkedList.LastNode();
	}

	//Pop Item from Queue
	template <typename T>
	T DynamicQueue<T>::Dequeue()
	{
		//check if queue reach to lower count or not
		if (IsEmpty())
		{
			//std::cout << "Queue Empty \n";
			return -1;
		}

		T topValue = _FrontPtr->Value;

		//Remove the first Node inside LinkedList
		_LinkedList.DeleteBegin();

		//Update Pointer of Top,Bottom to Refere to the Top Item , Bottom Item
		_FrontPtr = _LinkedList.FirstNode();


		if (IsEmpty())
		{
			_FrontPtr = nullptr;
			_RearPtr = nullptr;
		}

		//Reteurn the Value of Top Item
		return topValue;
	}

	//Peak Top Item from Queue
	template <typename T>
	T DynamicQueue<T>::Peak()
	{
		if (_FrontPtr != nullptr) 
		{
			return _FrontPtr->Value;
		}
		return nullptr;
	}

	//Print All Items
	template <typename T>
	void DynamicQueue<T>::Display()
	{

		if (IsEmpty())
		{
			//std::cout << "Queue is Empty";
		}
		else
		{
			_LinkedList.Display();
		}

	}

	//Print All Items
	template <typename T>
	void DynamicQueue<T>::DisplayReverse()
	{

		if (IsEmpty())
		{
			//std::cout << "Queue is Empty";
		}
		else
		{
			_LinkedList.DisplayReverse();
		}

	}

	//Clear All Items inside Queue
	template <typename T>
	void DynamicQueue<T>::Clear()
	{
		_LinkedList.Clear();
	}

#pragma endregion

}

#endif // !DynamicQueue_H_


