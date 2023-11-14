#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>


MODULE_AUTHOR("Olena Fedosieieva <fedosieieva.olena@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

//TASK II - A. Set parameter default to 1
static uint count = 1;

//TASK II - D. Add desription to parameter
module_param(count, uint, S_IRUGO);
MODULE_PARM_DESC(count, "Number of times to print \"Hello, world!\" (default=1)");

//TASK III - Declaring data sctucture with list_head and ktime_t elements
struct my_struct {
	struct list_head list;
	ktime_t kernel_time;
};

//TASK IV - Creating a static variable for head of the list
static LIST_HEAD(my_list);

static int __init hello_init(void){
	int i;
	struct my_struct *new_node;
	struct my_struct *pos;

	//TASK II - B. Add a warning if parameter = 0 or is in range 5 - 10
	if (count == 0 || (count >= 5 && count <= 10)){
		printk(KERN_WARNING "Warning: count parameter has been changed from default 1 to (%u).\n", count);
	}

	//TASK II - C. If parameter > 10 then print and return error
	if (count > 10){
		printk(KERN_ERR "Error: Count should be inside 0 - 10 range.\n");
		return -EINVAL;
	}

	//TASK V - Allocate memory for structure instance and
	//fill with kernel time using ktime_get()
	for(i = 0; i < count; i++){
		new_node = kmalloc(sizeof(struct my_struct), GFP_KERNEL);
		if (!new_node){
			printk(KERN_ERR "Error: couldn't allocate memory\n");
			return -ENOMEM;
		}

		INIT_LIST_HEAD(&new_node->list);
		new_node->kernel_time = ktime_get();
		list_add_tail(&new_node->list, &my_list);

		printk(KERN_INFO "Hello, world! Current kernel time: %lld\n", ktime_to_ns(new_node->kernel_time));
	}
	return 0;
}


static void __exit hello_exit(void){
	struct my_struct *pos, *tmp;
	printk(KERN_INFO "List of greeting and kernel times: \n");
	
	//TASK VI - Go through list, print actions with their times,
	//after which delete the element from list and free allocated memory
	list_for_each_entry(pos, &my_list, list){
		printk(KERN_INFO "Greeting with kernel time: %lld\n", ktime_to_ns(pos->kernel_time));
	}

	list_for_each_entry_safe(pos, tmp, &my_list, list){
		list_del(&pos->list);
		kfree(pos);
	}

	//Print "Goodbye, world" to see where the function ends
	printk(KERN_INFO "Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
