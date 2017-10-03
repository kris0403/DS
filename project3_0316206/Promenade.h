#include<iostream>
using namespace std;

class Person{
public:
	Person();
	Person(int i, int e);
	~Person();
	int search_degree();
private:
	Person *parent, *child, *sibling; // the pointers to his parent, child, sibling
	int id; // record Person's id
	int energy; // record Person's energy
	int degree;// record how many levels of this Person's children

	
	void union_person(Person *o);
	void copy_person(Person *o);
	void add_new_child(Person*);
	Person *search_parrent();
	Person* search_leftsibling();
    Person* find_id(int);

	friend class Promenade;//you could access all valuables in class Promenade
};

class Promenade{

public:
	Promenade();
	Promenade(Promenade*);
	~Promenade();

	void one_person_joined(int id, int energy);
	void one_group_joined(Promenade *g);
	void absorb_energy(int id, int energy);
	void out(Person* temp,int counter);
	int  calculate_people_below(int energy);
	int  get_weakest_person();
	int  size();

private:
	Person *head;
	int num;//caculate how many people in the promenade

	void adjust();
	size_t search_root_size();
	void  delete_weakest_person();
    Person* get_weakest_person_pointer();
    Person* search_parent(int key);
    bool is_root(int key);
};

//ctor
Person::Person(){
	parent = child = sibling = NULL;
	id = 0;
	energy = 0;
	degree = 0;
}

//ctor
Person::Person(int i, int e){// define a new Person with id = i and energy = e
	parent = child = sibling = NULL;
	id = i;
	energy = e;
	degree = 0;
}

//dtor
Person::~Person(){

}

int Person::search_degree(){
    return degree;
}



size_t Promenade::search_root_size(){
    if(head == NULL){
        return 0;
    }

    size_t root_size = 1;

    Person *temp = head->sibling;
    while(temp->id != head->id){
        temp = temp->sibling;
        root_size++;
    }

    return root_size;
}

void Promenade::adjust(){
    int adjust_degree = 0;
    int max_degree = head->degree + 3;
    while(adjust_degree <= max_degree){
        max_degree = head->degree;
        Person *temp = head->sibling;
        while(temp->id != head->id){
            if(max_degree < temp->degree)
                max_degree = temp->degree;
            temp = temp->sibling;
        }

        if(search_root_size() <= 1){
            break;
        }

        Person *first_adjust_root = NULL;
        temp = head;

        while(1){
            if(temp->degree == adjust_degree){
                if(first_adjust_root == NULL){
                    first_adjust_root = temp;
                }
                else{
                    temp->search_leftsibling()->sibling = temp->sibling;
                    first_adjust_root->union_person(temp);

                    first_adjust_root = NULL;
                    temp = head;
                    continue;
                }
            }

            temp = temp->sibling;

            if(temp->id == head->id){
                break;
            }
        }
        adjust_degree++;
    }
}


void Person::union_person(Person *t){
    if(degree != t->degree)
    {
        cout<<"union_person is error!!!"<<endl;
        throw;
    }
	if(energy <= t->energy){
        add_new_child(t);
    }
    else{
        Person *temp = new Person();
        temp->copy_person(t);
        t->copy_person(this);
        this->copy_person(temp);
        add_new_child(t);
    }
}

void Promenade::out(Person* temp,int counter)
{
    if(this->num == 0 || counter == this->num)
    {
        return;
    }

    if(temp == NULL)
    {
        temp = this->head;
    }

    cout<<temp->id<<" ";
    counter++;

    if(temp->child != NULL)
    {
        out(temp->child,counter);
    }

    if(temp->sibling != temp)
    {
        out(temp->sibling,counter);
    }
}

void Person::copy_person(Person *t){
    parent = t->parent;
    child = t->child;
    sibling = t->sibling;
    id = t->id;
    energy = t->energy;
    degree = t->degree;
}

void Person::add_new_child(Person *t){
    t->parent = this;

    if(this->child == NULL){
        child = t;
        child->sibling = child;
    }
    else{
        Person *temp = child->sibling;
        child->sibling = t;
        t->sibling = temp;
    }
    degree++;
}

Person* Person::search_leftsibling(){
    Person *temp = this;
    while(temp->sibling != this){
        temp = temp->sibling;
    }
    return temp;
}


//ctor
Promenade::Promenade(){
	head = NULL;
	num = 0;
}

//wait to change
Promenade::Promenade(Promenade *t){
    head = t->head;
	num = t->num;

}

//dtor
Promenade::~Promenade(){

}

void Promenade::one_person_joined(int id, int energy){
	//TODO: fill in the code to simulate a person with his id and energy to join the Promenade
	Promenade *g = new Promenade();
	g->head = new Person(id,energy);
	g->head->sibling = g->head;
	g->num = 1;
	one_group_joined(g);
}

void Promenade::one_group_joined(Promenade *group){
	//TODO: fill in the code to simulate one group to join the full promenade

    //////if head or g is empty
    if(head == NULL){
        head = group->head;
        num = group->num;
        return;
    }
    else if(group->head == NULL){
        return;
    }

    //////else head or g are not empty
	this->num += group->num;

    group->head->search_leftsibling()->sibling = head->sibling;
    head->sibling = group->head;
	this->adjust();
}

void Promenade::absorb_energy(int id, int energy){
	//TODO: Elizabeth absord the energy of the person with this id, this Person's energy will decrease by this number.
	//		If his energy becomes lower than or equal to zero, he will die and should be thrown out automatically

	Person* temp = head->find_id(id);

	if(temp != NULL){
        temp->energy -= energy;

        while(!is_root(temp->id) && this->search_parent(temp->id)->energy > temp->energy){
            Person* t_child = temp;
            Person* t_parent = this->search_parent(temp->id);

            int temp_energy = temp->energy;
            int temp_id = temp->id;
            t_child->energy = t_parent->energy;
            t_child->id = t_parent->id;
            t_parent->energy = temp_energy;
            t_parent->id = temp_id;

            temp = t_parent;

        }

        if(temp->energy <= 0){
            delete_weakest_person();
        }
    }


}


int Promenade::calculate_people_below(int energy){
	//TODO: You should calculate how many people's energy are below the number (include the number) and return it

	int energy_counter = 0;
    if(num == 0)return 0;

    if(head->energy <= energy)
        energy_counter++;
    Person *temp = head->sibling;
    while(temp->id != head->id){
        if(temp->energy <= energy)
            energy_counter++;
        temp = temp->sibling;
    }

    int counter = 1;
    int k_c = 1;
    while(counter <= this->num){
        Person* temp = this->head->find_id(k_c);
        if(temp !=NULL){
            if(temp->child != NULL){
                if(temp->child->energy <= energy)
                    energy_counter++;
                int t = temp->child->id;
                temp = temp->child->sibling;
                while(temp->id != t){
                    if(temp->energy <= energy)
                        energy_counter++;
                    temp = temp->sibling;
                }
            }
            counter++;
        }
        k_c++;
    }
	return energy_counter;
}

int Promenade::get_weakest_person(){
	return get_weakest_person_pointer()->id;
}

int Promenade::size(){
	return num;
}






void  Promenade::delete_weakest_person(){
    if(num == 0){
        return;
    }
    else if(num == 1){
        num--;
        head = NULL;
        return;
    }

    Person *weakest_person_pointer = get_weakest_person_pointer();
    weakest_person_pointer->search_leftsibling()->sibling = weakest_person_pointer->sibling;
    if(weakest_person_pointer->sibling != this->head){
        this->head =  weakest_person_pointer->sibling;
    }
    else{
        this->head = NULL;
    }
    Promenade *pr2 = new Promenade();
    if(weakest_person_pointer->child != NULL){
        pr2->head =  weakest_person_pointer->child;
    }
    else{
        pr2->head = NULL;}

    int t_num = this->num - 1;
    this->one_group_joined(pr2);
    this->num = t_num;


    delete weakest_person_pointer;
}

Person* Promenade::get_weakest_person_pointer(){
    int min_energe = head->energy;
    Person *weakest_person_pointer = head;

    int counter = 1;
    int k_c = 1;
    while(counter <= this->num){
        Person* temp = this->head->find_id(k_c);
        if(temp !=NULL){
            if(temp->energy < min_energe){
                weakest_person_pointer = temp;
                min_energe = temp->energy;
            }
            counter++;
        }
        k_c++;
    }

    return weakest_person_pointer;
}

Person* Promenade::search_parent(int energy)
{
    int counter = 1;
    int k_c = 1;
    while(counter <= this->num){
        Person* temp = this->head->find_id(k_c);
        Person* parent = temp;
        if(temp !=NULL){
            if(temp->child != NULL){
                if(temp->child->id == energy){
                    return temp;
                }

                int t = temp->child->id;
                temp = temp->child->sibling;
                while(temp->id != t){
                    if(temp->id == energy){
                        return parent;
                    }
                    temp = temp->sibling;
                }
            }
            counter++;
        }
        k_c++;
    }

    return NULL;
}

bool Promenade::is_root(int energy)
{
    if(num == 0)return false;
    if(head->id == energy)return true;

    Person *temp = head->sibling;
    while(temp->id != head->id){
        if(temp->id == energy)return true;
        temp = temp->sibling;
    }
    return false;
}


Person* Person::find_id(int key)
{
    if(id == key)
    {
        return this;
    }

    Person *temp = this->sibling;
    while(temp->id != this->id)
    {
        if(temp->id == key)
        {
            return temp;
        }
        else
        {
            if(temp->child != NULL)
            {
                Person *temp_find = temp->child->find_id(key);
                if(temp_find != NULL)
                {
                    return temp_find;
                }
            }
        }

        temp = temp->sibling;
    }

    if(this->child != NULL)
    {
        Person *temp_find = this->child->find_id(key);
        if(temp_find != NULL)
        {
            return temp_find;
        }
    }

    return NULL;

}




