#include <linux/module.h>           /* Defines functions such as module_init/module_exit */
#include <linux/gpio.h>             /* Defines functions such as gpio_request/gpio_free */
#include <linux/platform_device.h>  /* For platform devices */
#include <linux/gpio/consumer.h>    /* For GPIO Descriptor */
#include <linux/of.h>               /* For DT */  
#include <linux/interrupt.h>
#include <linux/fs.h>      /* Thu vien nay dinh nghia cac ham allocate major/minor number */
#include <linux/device.h>  /* Thu vien nay dinh nghia cac ham class_create/device_create */
#include <linux/cdev.h>    /* Thu vien nay dinh nghia cac ham kmalloc */
#include<linux/slab.h>     /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include<linux/uaccess.h>  /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user *//* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include <linux/poll.h>
static int my_pdrv_probe(struct platform_device *pdev);
static int my_pdrv_remove(struct platform_device *pdev);


static int __init Khoitao(void);
static void __exit Huy(void);


static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t  m_read(struct file *filp, char __user *user_buf, size_t size,loff_t * offset);
static ssize_t  m_write(struct file *filp, const char *user_buf, size_t size, loff_t * offset);
static unsigned int m_poll(struct file *file, poll_table *wait);

typedef struct {
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
    int size;
   // char kernel_buff[1];
    bool is_event_available; // Biến kiểm tra có sự kiện mới không
    wait_queue_head_t read_queue; // Hàng đợi cho các tiến trình đọc
} m_foo_dev;

char kernel_buff[1];

m_foo_dev mdev;

#define DRIVER_AUTHOR "tuantnt08@gmail.com"
#define DRIVER_DESC   "gpio test device tree"

#define LOW     0
#define HIGH    1

struct gpio_desc *gpio2_4;
struct gpio_desc *gpio2_5;
struct gpio_desc *gpio1_13;
struct gpio_desc *gpio1_12;
struct gpio_desc *gpio0_23;
int irq1, irq2, irq3, irq4, irq5;
int check ;

static irqreturn_t handler1(int irq, void *dev_id);
static irqreturn_t handler2(int irq, void *dev_id);
static irqreturn_t handler3(int irq, void *dev_id);
static irqreturn_t handler4(int irq, void *dev_id);
static irqreturn_t handler5(int irq, void *dev_id);

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "gpio_base_myself", },
    { /* sentinel */ }
};

/* platform driver */
static struct platform_driver mypdrv = {
    .probe = my_pdrv_probe,
    .remove = my_pdrv_remove,
    .driver = {
        .name = "descriptor-based",
        .of_match_table = of_match_ptr(gpiod_dt_ids),
        .owner = THIS_MODULE,
    },
};

MODULE_DEVICE_TABLE(of, gpiod_dt_ids);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = m_read,
  //  .write      = m_write,
    .open       = m_open,
    .poll       = m_poll,
    .release    = m_release,
};

static int my_pdrv_probe(struct platform_device *pdev)
{
    pr_info("Probe function\n");
    struct device *dev = &pdev->dev;
    gpio0_23 =  gpiod_get(dev, "button23", GPIOD_IN);
    gpio1_13 = gpiod_get(dev, "button45", GPIOD_IN);
    gpio1_12=  gpiod_get(dev, "button44", GPIOD_IN);
    gpio2_4=  gpiod_get(dev, "button68", GPIOD_IN);
    gpio2_5=  gpiod_get(dev, "button69", GPIOD_IN);

    gpiod_set_debounce(gpio0_23, 200);
    gpiod_set_debounce(gpio1_12, 200);
    gpiod_set_debounce(gpio1_13, 200);
    gpiod_set_debounce(gpio2_4, 200);
    gpiod_set_debounce(gpio2_5, 200);

    irq1 = gpiod_to_irq(gpio0_23);
    irq2 = gpiod_to_irq(gpio1_12);
    irq3 = gpiod_to_irq(gpio1_13);
    irq4 = gpiod_to_irq(gpio2_4);
    irq5 = gpiod_to_irq(gpio2_5);

    check = request_irq(irq1,handler1, IRQF_TRIGGER_FALLING, "button_irq1", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio0_23);
    return check;
    }

    check = request_irq(irq2,handler2, IRQF_TRIGGER_FALLING, "button_irq2", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio1_12);
    return check;
    }

    check = request_irq(irq3,handler3, IRQF_TRIGGER_FALLING, "button_irq3", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio1_13);
    return check;
    }

    check = request_irq(irq4,handler4, IRQF_TRIGGER_FALLING, "button_irq4", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio2_4);
    return check;
    }

    check = request_irq(irq5,handler5, IRQF_TRIGGER_FALLING, "button_irq5", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio2_5);
    return check;
    }

    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    pr_info("remove function\n");

    free_irq(irq1, NULL);
    gpiod_put(gpio0_23);

    free_irq(irq2, NULL);
    gpiod_put(gpio1_12);

    free_irq(irq3, NULL);
    gpiod_put(gpio1_13);
    free_irq(irq4, NULL);
    gpiod_put(gpio2_4);

    free_irq(irq5, NULL);
    gpiod_put(gpio2_5);
    pr_info("End remove\n");

    return 0;
}

static irqreturn_t handler1(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio0_23);
    
    if (pressed)
        pr_info("Button1 Pressed\n");
    else
        pr_info("Button1 Released\n");
    
    kernel_buff[0] = '1';
    mdev.is_event_available = true;
    wake_up_interruptible(&mdev.read_queue);
    return IRQ_HANDLED;
}

static irqreturn_t handler2(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio1_12);
    
    if (pressed)
        pr_info("Button2 Pressed\n");
    else
        pr_info("Button2 Released\n");

    kernel_buff[0] = '2';
    mdev.is_event_available = true;
    wake_up_interruptible(&mdev.read_queue);
    return IRQ_HANDLED;
}

static irqreturn_t handler3(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio1_13);
    
    if (pressed)
        pr_info("Button3 Pressed\n");
    else
        pr_info("Button3 Released\n");
    
    kernel_buff[0] = '3';
    mdev.is_event_available = true;
    wake_up_interruptible(&mdev.read_queue);
    return IRQ_HANDLED;
}

static irqreturn_t handler4(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio2_4);
    
    if (pressed)
        pr_info("Button4 Pressed\n");
    else
        pr_info("Button4 Released\n");

    kernel_buff[0] = '4';
    mdev.is_event_available = true;
    wake_up_interruptible(&mdev.read_queue);
    return IRQ_HANDLED;
}

static irqreturn_t handler5(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio2_5);
    
    if (pressed)
        pr_info("Button5 Pressed\n");
    else
        pr_info("Button5 Released\n");

    kernel_buff[0] = '5';
    mdev.is_event_available = true;
    wake_up_interruptible(&mdev.read_queue);
    return IRQ_HANDLED;
}

/* This function will be called when we open the Device file */
static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");
    return 0;
}

/* This function will be called when we close the Device file */
static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");
    return 0;
}

/* This function will be called when we read the Device file */
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset)
{
    ssize_t result = 0;

    // Chờ cho đến khi có sự kiện (nút được nhấn)
    wait_event_interruptible(mdev.read_queue, mdev.is_event_available);

    // Kiểm tra xem đã có dữ liệu để đọc chưa
    if (!mdev.is_event_available) {
        // Nếu không có sự kiện nào mới, trả về 0 để báo hiệu EOF (kết thúc file)
        return 0;
    }

    // Copy dữ liệu từ kernel buffer sang user buffer
    if (copy_to_user(user_buf, kernel_buff, 1)) {
        // Trả về lỗi nếu không copy được
        result = -EFAULT;
    } else {
        // Đánh dấu đã đọc xong và trả về số byte đã đọc
        mdev.is_event_available = false; // Reset lại cờ sự kiện
        result = 1; // Trả về số byte đã copy
    }

    return result;

}


static unsigned int m_poll(struct file *file, poll_table *wait)
{
    unsigned int reval_mask = 0;
    poll_wait(file, &mdev.read_queue, wait);
    if (mdev.is_event_available)
    {
        reval_mask |= POLLIN | POLLRDNORM; // Dữ liệu đọc sẵn có
    }
    //pr_info("Nodata send\n");
    return reval_mask;
}

static int __init Khoitao(void)
{
     if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "my-cdev"))
    {
        pr_err("ERROR: Can not make number device\n");
        return -1;
    }
    pr_info("Kho tao : bat dau\n");
    
    pr_info("Major : %d    Minor : %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));
    
    if ((mdev.m_class = class_create(THIS_MODULE, "my_class")) == NULL){
        pr_err("ERROR: Can not create class\n");
        goto rm_dev_num;
    }
    pr_info("Init : Khoi tao class create\n");

    if (device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "my_button_device") == NULL)
    {
        pr_err("ERROR: Can not create device\n");
        goto rm_class;

    }
    pr_info("Init : Khoi tao device create\n");

    cdev_init(&mdev.m_cdev, &fops);
    pr_info("Init : Khoi tao Cdev Init\n");
    if (cdev_add(&mdev.m_cdev, mdev.dev_num, 1) < 0)
    {
        pr_err("ERROR: Can not add device\n");
        goto rm_dev_num;
    }
    pr_info("Init : Khoi tao Cdev add\n");

    platform_driver_register(&mypdrv);
    pr_info("register platform device\n");

    init_waitqueue_head(&mdev.read_queue);
    mdev.is_event_available = false;

    pr_info("Khoi tao ket thuc\n");

    return 0;

rm_dev_num:
    unregister_chrdev_region(mdev.dev_num,1);
rm_class:
    class_destroy(mdev.m_class);

    return -1;
}

static void __exit Huy(void) {
    pr_info("Start Huy\n");
    platform_driver_unregister(&mypdrv);
    pr_info("unregister platform device\n");

    cdev_del(&mdev.m_cdev); 
    pr_info("Exit: huy cdev");

    device_destroy(mdev.m_class, mdev.dev_num);
    pr_info("Exit: huy device_destroy\n");

    class_destroy(mdev.m_class);
    pr_info("Exit: check huy class_destroy\n");


    unregister_chrdev_region(mdev.dev_num, 1);
    pr_info("Exit: check huy number\n");

    pr_info("End Huy\n");
}

module_init(Khoitao);
module_exit(Huy);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");