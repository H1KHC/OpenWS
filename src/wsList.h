#ifndef __WS_LIST_H__
#define __WS_LIST_H__

template <class T>
struct wsListNode {
	T data;
	wsListNode *last, *next;
	wsListNode(T a) : data(a), last(nullptr), next(nullptr) {};
};

template <class T>
class wsList {
private:
	wsListNode<T> *head, *tail;

	static wsListNode<T> *cut(wsListNode<T> *ptr) {
		if (ptr->last) ptr->last->next = ptr->next;
		if (ptr->next) ptr->next->last = ptr->last;
		ptr->last = ptr->next = nullptr;
		return ptr;
	}
	static wsListNode<T>* linkAfter(wsListNode<T> *ptr, wsListNode<T> *node) {
		if (ptr->next) ptr->next->last = node;
		node->next = ptr->next;
		node->last = ptr;
		ptr->next = node;
		return node;
	}
	static wsListNode<T>* linkBefore(wsListNode<T> *ptr, wsListNode<T> *node) {
		if (ptr->last) ptr->last->next = node;
		node->last = ptr->last;
		node->next = ptr;
		ptr->last = node;
		return node;
	}

public:
	const wsListNode<T> *front() const { return head; }
	const wsListNode<T> *back() const { return tail; }
	wsListNode<T> *front() { return head; }
	wsListNode<T> *back() { return tail; }

	void addFront(T data) {
		if (!head) {
			head = tail = new wsListNode<T>(data);
			return;
		}
		head = linkBefore(head, new wsListNode<T>(data));
	}
	void addBack(T data) {
		if (!head) {
			head = tail = new wsListNode<T>(data);
			return;
		}
		tail = linkAfter(tail, new wsListNode<T>(data));
	}
	void putFront(wsListNode<T> *pos) {
		if (pos == head) return;
		if (pos == tail) tail = tail->last;
		cut(pos);
		head = linkBefore(head, pos);
	}
	void putBack(wsListNode<T> *pos) {
		if (pos == tail) return;
		if (pos == head) head = head->next;
		cut(pos);
		tail = linkAfter(tail, pos);
	}
	void deleteFront() {
		if (head) {
			wsListNode<T> *node = head;
			head = head->next;
			if (!head) tail = nullptr;
			cut(node);
			delete node;
		}
	}
	void deleteBack() {
		if (tail) {
			wsListNode<T> *node = tail;
			tail = tail->last;
			if (!tail) head = nullptr;
			cut(node);
			delete node;
		}
	}
	static void deleteNode(wsListNode<T> *pos) {
		if (pos == nullptr) return;
		cut(pos);
		delete pos;
	}
	void deleteNode(const T &val) {
		wsListNode<T> *pos = find(val);
		if (!pos) return;
		if (pos == head)
			deleteFront();
		else if (pos == tail)
			deleteBack();
		else {
			cut(pos);
			delete pos;
		}
	}

	wsListNode<T> *find(const T &val) {
		for (wsListNode<T> *node = head; node; node = node->next) {
			if (node->data == val) {
				return node;
			}
		}
		return nullptr;
	}

	int isEmpty() {
		return head == nullptr;
	}

	wsList() : head(nullptr), tail(nullptr) {};
};

#endif
