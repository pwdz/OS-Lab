#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
};


static LIST_HEAD(birthday_list);

int simple_init(void){
    int day = 1;
    int year = 2077;
    int month = 3;
    struct birthday *ptr;
    struct birthday *person;
    person = kmalloc(sizeof(person), GFP_KERNEL);
    person->day = day;
    person->month = month;
    person->year = year;

    INIT_LIST_HEAD(&person->list);
    list_add_tail(&person->list, &birthday_list);


    for (month=1; month<5; month++ ){
        person = kmalloc(sizeof(person), GFP_KERNEL);
        person->day = ++day;
        person->month = month;
        person->year = ++year;

        list_add_tail(&person->list, &birthday_list);
	}
	
	list_for_each_entry(ptr, &birthday_list, list) {
        printk(KERN_INFO "birthdate: %d/%d/%d\n", ptr->year, ptr->month, ptr->day);
    }
    printk(KERN_INFO "Loading Module\n");
	return 0;
}

void simple_exit(void)
{
    printk(KERN_INFO "Removing List nodes and module\n");
	struct birthday *ptr;
	list_for_each_entry(ptr, &birthday_list, list) {
        kfree(ptr);
    }
    printk(KERN_INFO "Module removed\n");
}

module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Birthday Mud");
MODULE_AUTHOR("ebi");


