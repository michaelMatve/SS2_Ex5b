#include "OrgChart.hpp"

namespace ariel{

//////////////////////////////////////////////////////////////////
///////////////OrgChart class ////////////////////////////////////
//////////////////////////////////////////////////////////////////

    OrgChart::OrgChart()
    {
        this->root = nullptr;//start root to null ptr
    }

    //change root
    OrgChart& OrgChart::add_root(std::string data)
    {
        //if the root is null make new node to the root
        if(this->root == nullptr)
        {
            this->root = new ONode{data};
        }
        //change the data
        this->root->data = data;
        return *this;
    }

    //add children
    OrgChart& OrgChart::add_sub(std::string father, std::string son)
    {
        //find the father
        ONode* pointer_father = this->find(father);
        //if the pather not found throw exp
        if(pointer_father == nullptr)
        {
            throw std::invalid_argument("not find");
        }
        // make new node to the son
        ONode* pointer_son = new ONode(son);
        //set the son father
        pointer_son->father = pointer_father;
        //set the new son node as a fathers son as last shild
        if(pointer_father->child == nullptr)
        {
            //if the father as no child put this node as child
            pointer_father->child = pointer_son ;
        }
        else
        {
            //run on all the child antill the end and put it in the end
            pointer_father = pointer_father->child;
            while(pointer_father->brother != nullptr)
            {
                pointer_father = pointer_father->brother;
            }
            pointer_father->brother = pointer_son;
        }
        return *this;
    }

    OrgChart::ONode* OrgChart::find(std::string data)
    {
        //run on the level order operator and check if the data is the same 
        for (auto it = this->begin_level_order(); it != this->end_level_order(); ++it)
        {
            if(*it == data)
            {
                //return the node with the same data
                return it.get_ptr();
            }
        }
        return nullptr;
    }

    std::ostream& operator<<(std::ostream& My_output,const OrgChart& tree)
    {
        //start
        My_output<<"-----------------start----------"<<"\n";
        //counter to remember in what level we are
        int colums =0;

        OrgChart::ONode* curr_node = tree.root;
        //add the root to the my_output
        My_output<<" ("<<colums<< ") "<< curr_node->data;
        //run on all the nodes in predetor order and print them in to my output
       while(curr_node!= nullptr)
        {
            //run to the last level
            while(curr_node->child != nullptr)
            {
                curr_node= curr_node->child;
                ++colums;
                My_output<< " -> " <<" ("<<colums<< ") "<<curr_node->data;
            }
            //after finish go up to the prvius level and check if there is brother
            while(curr_node->brother == nullptr)
            {
                curr_node = curr_node->father;
                //if we get to null stop
                if(curr_node == nullptr)
                {
                    break;
                }
                --colums;
            }
            if(curr_node != nullptr)
            {
                curr_node = curr_node->brother;
                My_output<< "\n";
                for(ulong i= 0 ; i<colums ; i++)
                {
                   My_output<<" \t  ";  
                }
                My_output<< " -> " <<" ("<<colums<< ") "<<curr_node->data;
            }
        }

        My_output<<"\n"<<"-----------------end----------"<<std::endl;
        return My_output;
    }
    //iterators start and end
    OrgChart::Iterator OrgChart::begin_level_order()
    {
        //return the start of the itrshion and set a vector with the true order
        return Iterator(this->root).full_tor();
    }

    OrgChart::Iterator OrgChart::end_level_order()
    {
        return nullptr;
    }
    
    OrgChart::Iterator OrgChart::begin_reverse_order()
    {
        //return the start of the itrshion and set a vector with the true order
        return Iterator(this->root).full_stack();
    }

    OrgChart::Iterator OrgChart::reverse_order()
    {
        return nullptr;
    }

    OrgChart::Iterator OrgChart::begin_preorder()
    {
        //return the start of the itrshion and set a vector with the true order
        return Iterator(this->root).full_vec();
    }

    OrgChart::Iterator OrgChart::end_preorder()
    {
        return nullptr;
    }
 
    OrgChart::Iterator OrgChart::begin()
    {
        return this->begin_level_order();
    }

    OrgChart::Iterator OrgChart::end()
    {
        return this->end_level_order();
    }
 
    std::string& OrgChart::Iterator::operator*()
    {
        return this->ptr->data;
    }

    std::string* OrgChart::Iterator::operator->()
    {
        return &(this->ptr->data);
    }
    
    OrgChart::Iterator& OrgChart::Iterator::operator++()
    {
        //change the curr place
        ++this->place;
        //if the place is out of the vector range we finish end return null
        if(this->place == this->ptr_vec.size())
        {
            this->ptr = nullptr;
        }
        //else set this->ptr to the ptr_vec in the place
        else
        {
            this->ptr = this->ptr_vec[this->place];
        }
        return *this;
    }
    
    OrgChart::Iterator OrgChart::Iterator::operator++(int)
    {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }
    
    bool OrgChart::Iterator::operator!=(const Iterator &other) const
    {
        //check the pointers are not the same
        if(this->ptr!=other.ptr)
        {
            return true;
        }
        return false;
    }

    OrgChart::Iterator& OrgChart::Iterator::full_tor()
    {
        //check the tree is not null
        if(this->ptr == nullptr)
        {
            return *this;
        }
    //add the root to the vector
       this->ptr_vec.push_back(this->ptr);
       ONode* curr_node  = this->ptr;
       ulong tor_place = 0;
        //run on all the nodes in the vector and add there cildred to the vector
       while(tor_place < this->ptr_vec.size())
       {   
           curr_node = this->ptr_vec[tor_place]->child;
           while(curr_node != nullptr)
           {
               this->ptr_vec.push_back(curr_node);
               curr_node = curr_node->brother;
           }
           ++tor_place;
       }
       return *this;
    }

    OrgChart::Iterator& OrgChart::Iterator::full_stack()
    {
        //check the tree is not null
        if(this->ptr == nullptr)
        {
            return *this;
        }
        //add the root at the begining
       this->ptr_vec.insert(this->ptr_vec.begin(),this->ptr);
       ONode* curr_node  = this->ptr;
       ulong tor_place = 0;
        //run on all the nodes in the vector from the end and add all the children of them in the begining
        while(tor_place < this->ptr_vec.size())
        {   
           curr_node = this->ptr_vec[this->ptr_vec.size()- tor_place -1]->child;
           //add the children in there true order
           int index =0;
           while(curr_node != nullptr)
           {
               this->ptr_vec.insert(this->ptr_vec.begin()+ index , curr_node);
               curr_node = curr_node->brother;
               ++index;
           }
           ++tor_place;
       }
       this->ptr = this->ptr_vec[0];
       return *this;
    }

    OrgChart::Iterator& OrgChart::Iterator::full_vec()
    {
        ONode* curr_node = this->ptr;
        //add the root to vector
        this->ptr_vec.push_back(curr_node);
        //run antil we get to nullptr
        while(curr_node!= nullptr)
        {
            //go to the last child of child and add it to the vector
            while(curr_node->child != nullptr)
            {
                curr_node= curr_node->child;
                this->ptr_vec.push_back(curr_node);
            }
            //go back in the levels antil get to one that has brother
            while(curr_node->brother == nullptr)
            {
                curr_node = curr_node->father;
                //if got to null finished
                if(curr_node == nullptr)
                {
                    break;
                }
            }
            //if found node with brother go to the brother add him to the vector and check his children
            if(curr_node != nullptr)
            {
                curr_node= curr_node->brother;
                this->ptr_vec.push_back(curr_node);
            }
        }
        return *this;
    }
    //return the iterators pointer to the node
    OrgChart::ONode* OrgChart::Iterator::get_ptr()
    {
        return this->ptr;
    }
}