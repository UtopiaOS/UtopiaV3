

struct user {
	char *name;
	int age;
};

int main() {
	struct user* user = c_std_alloc((sizeof(struct user*)), sizeof(unsigned char));
	c_std_exit(0);
}
