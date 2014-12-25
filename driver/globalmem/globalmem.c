/**
  file��globalmem.c
  author��zuokongxiao
  �ַ��豸��������ģ��
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/timer.h>
#include <linux/gpio.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

//��С����4K
#define GLOBALMEM_SIZE          4096
//���豸�Ŷ���
#define GLOBALMEN_MAJOR         254
//�豸������
#define GLOBALMEN_NAME          "globalmem"

static globalmem_major = GLOBALMEN_MAJOR;
//�豸�ṹ�嶨��
struct globalmem_dev
{
    struct cdev cdev; /* cdev�ṹ�� */
    unsigned char mem[GLOBALMEM_SIZE];
};
//�豸�ṹ��ָ�붨��
struct globalmem_dev *globalmen_devp;








/* �豸�� */
#define DEVICE_NAME		"bb_led"

/* led */
static const unsigned long bb_led_tab[] = {53, 54, 55, 56};
struct timer_list s_stTimer;
int lastio = 0;
dev_t led_devno;
struct cdev led_cdev;


/* ��ʱ����ִ�еĺ��� */
void timer_cb(unsigned long ulArg)
{
	int io = 0;

	//printk("Time up!\n");
	mod_timer(&s_stTimer, TIMER_EXPIRES);
	io = gpio_get_value(bb_led_tab[3]);
	//printk("io=%d\r\n", io);
	if(0 != lastio)
	{
		lastio = 0;
	}
	else
	{
		lastio = 1;
	}
	gpio_set_value(bb_led_tab[3], lastio);
}

/* open */
int bb_led_open(struct inode *inode, struct file *filp)
{
	printk("led device is open!\r\n");
	return 0;
}

/* release */
int bb_led_release(struct inode *inode, struct file *filp)
{
	printk("led device is release!\r\n");
	return 0;
}

/* ioctl */
long bb_led_ioctl(struct file *file, unsigned int cmd, unsigned long arg) 
{
	long ret = 0;
	
	printk("ioctl fn: cmd=%d\r\n", cmd);
	switch(cmd)
	{
		case 0:
			gpio_set_value(bb_led_tab[1], 0);
			break;
		case 1:
			gpio_set_value(bb_led_tab[1], 1);
			break;
		default:
			ret = -1;
			break;
	}
	
	return ret;
}

/* �ļ������ṹ�� */
static const struct file_operations globalmen_fops = 
{
    .owner = THIS_MODULE,
    .llseek = ;
    .read = ;
    .write = ;
    .ioctl = ;
    .open = ;
    .release = ;
};

/* ��ʼ����ע��cdev */
static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
    int err = 0, devno = MKDEV(globalmem_major, index);

    cdev_init(&(dev->cdev), &globalmen_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &globalmen_fops;
    err = cdev_add(&(dev->cdev), devno, 1);
    if(err)
    {
        printk(KERN_NOTICE"cdev_add err = %d, devno = %d.\r\n", err, devno);
    }
}

/* ģ����غ��� */
static int __init globalmem_init(void)
{
	int ret = 0, err = 0;
	//int dev_no = 0;

	/* ��̬�����豸�� */
	ret = alloc_chrdev_region(&led_devno, 0, 1, "bb_led");
	if(0 > ret)
	{
		printk("alloc_chrdev_region return < 0\r\n");
		return -1;
	}
	//dev_no = MAJOR(led_devno);
	
	cdev_init(&led_cdev, &led_dev_fops);
	led_cdev.owner = THIS_MODULE;
	led_cdev.ops = &led_dev_fops;
	//err = cdev_add(&led_cdev, MKDEV(dev_no, 0), 1);
	err = cdev_add(&led_cdev, led_devno, 1);
	if(err)
	{
		printk("err=%d adding led_cdev!\r\n", err);
	}
	else
	{
		printk("success adding led_cdev!\r\n");
	}
	
	gpio_set_value(bb_led_tab[3], 1);
	gpio_set_value(bb_led_tab[1], 1);
	/* ��ʼ����ʱ�� */
	init_timer(&s_stTimer);
	s_stTimer.expires = TIMER_EXPIRES;
	s_stTimer.function = timer_cb;
	s_stTimer.data = 0;
	add_timer(&s_stTimer);
	printk("led driver init!\n");
	
	return 0;
}

/* ģ��ж�غ��� */
static void __exit globalmen_exit(void)
{
	//int dev_no = MAJOR(led_devno);;
	
	gpio_set_value(bb_led_tab[3], 0);
	gpio_set_value(bb_led_tab[1], 0);
	del_timer(&s_stTimer);
	
	cdev_del(&led_cdev);
	//unregister_chrdev_region(MKDEV(dev_no, 0), 1);
	unregister_chrdev_region(led_devno, 1);
	
	printk("led driver exit!\n");
}

module_init(globalmen_init);
module_exit(globalmen_exit);

MODULE_AUTHOR("zuokong");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Globalmem is char driver module.");



