	
	static NodeRef removeNode(NodeRef node, const T &data) {
		if (node == nullptr) return node;
		else if (data < node->data)
			node->left = removeNode(node->left, data);
		else if (data > node->data)
			node->right = removeNode(node->right, data);
		else {
			if (node->left == nullptr && node->right == nullptr) {
				delete node;
				node = nullptr;
			} else if (node->left == nullptr) { //one child
				NodeRef tmp = node;
				node = node->right;
				delete tmp;
			} else if (node->right == nullptr) { //one child
				NodeRef tmp = node;
				node = node->left;
				delete tmp;
			} else { //both children
				NodeRef tmp = min(node->right);
				node->data = tmp->data;
				node->right = removeNode(node->right, tmp->data);
			}
		} return node;
	}
	
	//Helps determine whether or not a particular node exists
	static NodeRef findNode(const T &data, NodeRef node) {
		if (data < node->data) {
			if (node->left == nullptr) return nullptr;
			else return findNode(data, node->left);
		} else if (data > node->data) {
			if (node->right == nullptr) return nullptr;
			else return findNode(data, node->right);
		} else return node;
	}
