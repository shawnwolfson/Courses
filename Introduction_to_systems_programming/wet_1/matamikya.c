#include "matamikya.h"
#include "amount_set.h"
#include "matamikya_print.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define SINGLE_UNIT 1
#define FIRST_LOWER_LETTER 'a'
#define LAST_LOWER_LETTER 'z'
#define FIRST_CAPITAL_LETTER 'A'
#define LAST_CAPITAL_LETTER 'Z'
#define FIRST_DIGIT '0'
#define LAST_DIGIT '9'
#define MINIMAL_ERROR 1
#define REMAINDER 500
#define LARGE_FACTOR 1000
#define FIRST_ELEMENT_IS_BIGGER 1
#define ELEMENTS_ARE_EQUAL 0
#define SECOND_ELEMENT_IS_BIGGER -1
#define FAILURE 0


struct Matamikya_t 
{
    AmountSet warehouse_products; //implement products manager using an amountset
    AmountSet warehouse_orders; //implement orders manager using an amountset
};

        
        //***new type -products- and it's functions***//

typedef struct Product_t{
    char* product_name;
    int product_id;
    double product_income;
    MtmProductData product_custom_data;
    MatamikyaAmountType product_amount_type;
    MtmGetProductPrice product_prodPrice;
    MtmCopyData product_custom_data_copy_function;
    MtmFreeData product_free_custom_data_function;
    CopyASElement productCopy;
    FreeASElement productFree;
    CompareASElements productCompare;
} *Product;

ASElement productCopy(ASElement product)
{
    if(product == NULL)
    {
        return NULL;
    }
    Product current_product = (Product)product;
    Product product_copy = malloc(sizeof(*product_copy));
    if(product_copy == NULL)
    {
        return NULL;
    }
    product_copy->product_name = malloc(strlen(current_product->product_name) + 1);
    if(product_copy->product_name == NULL)
    {
        free(product_copy);
        return NULL;
    }
    strcpy(product_copy->product_name, current_product->product_name);
    product_copy->product_id = current_product->product_id;
    product_copy->product_custom_data = current_product->product_custom_data_copy_function
                                                        (current_product->product_custom_data);
    product_copy->product_amount_type = current_product->product_amount_type;
    product_copy->product_prodPrice = current_product->product_prodPrice;
    product_copy->product_income = current_product->product_income;
    product_copy->product_custom_data_copy_function = current_product->product_custom_data_copy_function;
    product_copy->product_free_custom_data_function = current_product->product_free_custom_data_function;
    product_copy->productCopy = current_product->productCopy;
    product_copy->productFree = current_product->productFree;
    product_copy->productCompare = current_product->productCompare;
    return product_copy;
}

void productFree(ASElement product)
{
    if(product == NULL)
    {
        return;
    }
    Product casted_product = (Product)product;
    free(casted_product->product_name);
    casted_product->product_free_custom_data_function(casted_product->product_custom_data);
    free(product);
}

int productCompare(ASElement product_1, ASElement product_2)
{
    Product casted_product_1 = (Product)product_1;
    Product casted_product_2 = (Product)product_2;
    int id_1 = casted_product_1->product_id;
    int id_2 = casted_product_2->product_id;
    int result = 0;
    if(id_1 > id_2)
    {
        return FIRST_ELEMENT_IS_BIGGER;
    }
    else if(id_1 < id_2)
    {
        return SECOND_ELEMENT_IS_BIGGER;
    }
    else
    {
        return ELEMENTS_ARE_EQUAL;
    }
    return result;
}
            //******//



        //***new type -orders- and it's functions***//

typedef struct Order_t{
    unsigned int order_id;
    AmountSet order_items;
    CopyASElement orderCopy;
    FreeASElement orderDestroy;
    CompareASElements orderCompare;
} *Order;

ASElement orderCopy(ASElement order)
{
    if(order == NULL)
    {
        return NULL;
    }
    Order casted_order = (Order)order;
    Order order_copy = malloc(sizeof(*order_copy));
    if(order_copy == NULL)
    {
        return NULL;
    }
    order_copy->order_id = casted_order->order_id;
    order_copy->order_items = asCopy(casted_order->order_items);
    order_copy->orderCopy = casted_order->orderCopy;
    order_copy->orderDestroy = casted_order->orderDestroy;
    order_copy->orderCompare = casted_order->orderCompare;
    return order_copy;
}

void orderDestroy(ASElement order)
{
    if(order == NULL)
    {
        return;
    }
    Order casted_order = (Order)order;
    asDestroy(casted_order->order_items);
    free(order);
}

int orderCompare(ASElement order_1, ASElement order_2)
{
    Order casted_order_1 = (Order)order_1;
    Order casted_order_2 = (Order)order_2;
    int id_1 = casted_order_1->order_id;
    int id_2 = casted_order_2->order_id;
    if(id_1 > id_2)
    {
        return FIRST_ELEMENT_IS_BIGGER;
    }
    else if(id_1 < id_2)
    {
        return SECOND_ELEMENT_IS_BIGGER;
    }
    else
    {
        return ELEMENTS_ARE_EQUAL;
    }
}
            //******//


        //***static function for internal use***//

static bool checkIfNameIsValid(const char* name)
{
    bool result = false;
    if((*name >= FIRST_LOWER_LETTER) && (*name <= LAST_LOWER_LETTER)) //first letter is a small letter
    {
        result = true;
    }
    else if((*name >= FIRST_CAPITAL_LETTER) && (*name <= LAST_CAPITAL_LETTER)) //first letter is a capital letter
    {
        result = true;
    }
    else if((*name >= FIRST_DIGIT) && (*name <= LAST_DIGIT)) //first letter is a number
    {
        result = true;
    }
    return result;
}

static bool checkIfNameIsEmpty(const char* name)
{
    bool result = false;
    char* empty_string = "";
    if(strcmp(empty_string, name) == 0)
    {
        result = true;
    }
    return result;
}

static double my_abs(double number)
{
    return number > 0 ? number : (0 - number);
}

static bool checkIfAmountTypeIsConsistent(const double amount, int amountType)
{
    bool result = false;
    double temp_amount = amount;
    double rounded_amount = round(amount); //round the number
    double enlarged_rounded_amount = rounded_amount * LARGE_FACTOR;
    double enlarged_temp_amount = temp_amount * LARGE_FACTOR;
    switch (amountType)
    {
    case MATAMIKYA_INTEGER_AMOUNT:
        {
            double diff = my_abs(enlarged_rounded_amount - enlarged_temp_amount);
            if(diff <= MINIMAL_ERROR){ // check if the distance between rounded number and original is valid.
                result = true;
            }
            break;
        }
    case MATAMIKYA_HALF_INTEGER_AMOUNT:
        {
            bool number_was_rounded_up = (rounded_amount > amount ? true : false);
            double enlarged_rounded_amount_with_half = 0;
            if(number_was_rounded_up){
                enlarged_rounded_amount_with_half = enlarged_rounded_amount - REMAINDER;
            }
            else{
                enlarged_rounded_amount_with_half = enlarged_rounded_amount + REMAINDER;
            }
            if(((my_abs(enlarged_rounded_amount - enlarged_temp_amount)) <= MINIMAL_ERROR) ||
                (my_abs(enlarged_rounded_amount_with_half - enlarged_temp_amount)) <= MINIMAL_ERROR){
                result = true;
            }
            break;
        }
    case MATAMIKYA_ANY_AMOUNT:
            result = true;
            break;
    default:
        break;
    }
    return result;
}

static unsigned int generateNewId(Matamikya matamikya)
{
    int previous_id=0;
    AS_FOREACH(Order, iterator, matamikya->warehouse_orders)
    {
        if (iterator->order_id <= previous_id) {
            return (previous_id + 1);
        }
        else {
        previous_id=iterator->order_id;
        }
    }
    return (previous_id+1);
}

static Order getOrderById(AmountSet set, const unsigned int orderId)
{
    AS_FOREACH(Order, iterator, set)
    {
        if (iterator->order_id==orderId){
            return iterator;
        }
    }
    return NULL;
}

static Product getProductById(AmountSet set, const unsigned int productId)
{
    AS_FOREACH(Product, iterator, set)
    {
        if (iterator->product_id==productId){
            return iterator;
        }
    }
    return NULL;
}

static bool checkInventory(AmountSet warehouse_products, Order current_order, const unsigned int orderId)
{
    bool shortage=false;
    AS_FOREACH(Product, order_iterator, current_order->order_items){
        AS_FOREACH(Product, iterator, warehouse_products){
            if (!(iterator->productCompare(iterator, order_iterator))){
                double warehouse_amount=0;
                double order_amount=0;
                asGetAmount(warehouse_products, iterator, &warehouse_amount);
                asGetAmount(current_order->order_items, order_iterator, &order_amount);
                if (warehouse_amount < order_amount){
                    shortage=true;
                    return shortage;
                }
            }
        }
    }
    return shortage;
}

static void shipItem(Matamikya matamikya, Product product, double amount)
{
    (product->product_income) += product->product_prodPrice(product->product_custom_data, amount) ;
    asChangeAmount(matamikya->warehouse_products, product, 0-amount);
}

static double calculateTotalPrice(Order order)
{
    double sum=0;
    AS_FOREACH(Product, iterator, order->order_items){
        double current_amount=0;
        asGetAmount(order->order_items, iterator, &current_amount);
        sum += (iterator->product_prodPrice(iterator->product_custom_data, current_amount));
    }
    return sum;
}

static Product getBestSeller(Matamikya matamikya)
{
    Product best_seller = asGetFirst(matamikya->warehouse_products);
    AS_FOREACH(Product, iterator, matamikya->warehouse_products)
    {
        if ((iterator->product_income) > (best_seller->product_income)) {
            best_seller=iterator;
        }
    }
    return best_seller;
}
            //******//



        //***interface functions***//

//create an empty Matamikya warehouse.
Matamikya matamikyaCreate()
{
    Matamikya new_warehouse = malloc(sizeof(*new_warehouse));
    if(new_warehouse == NULL)
    {
        return NULL;
    }
    new_warehouse->warehouse_products = asCreate(productCopy, productFree, productCompare); 
    new_warehouse->warehouse_orders = asCreate(orderCopy, orderDestroy, orderCompare);
    if((new_warehouse->warehouse_products == NULL) || (new_warehouse->warehouse_orders == NULL))
    {
        return NULL;
    }
    return new_warehouse;
}

//free a Matamikya warehouse, and all its contents from memory.
void matamikyaDestroy(Matamikya matamikya)
{
    if(matamikya == NULL)
    {
        return;
    }
    asDestroy(matamikya->warehouse_orders);
    asDestroy(matamikya->warehouse_products);

    free(matamikya);
}

//add a new product to a Matamikya warehouse
MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    if((matamikya == NULL) || (name == NULL) || (customData == NULL)
        || (copyData == NULL) || (freeData == NULL) || (prodPrice == NULL)) 
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if(checkIfNameIsEmpty(name) || !(checkIfNameIsValid(name)))
    {
        return MATAMIKYA_INVALID_NAME;
    }
    if((amount < 0) || !(checkIfAmountTypeIsConsistent(amount, amountType))) 
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }
    //if everything is ok, create a new product
    Product new_product = malloc(sizeof(*new_product));
    if(new_product == NULL)
    {
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    new_product->product_name = malloc(strlen(name) + 1);
    if(new_product->product_name == NULL)
    {
        free(new_product);
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    strcpy(new_product->product_name, name);
    new_product->product_id = id;
    new_product->product_income = 0;
    new_product->product_amount_type = amountType;
    new_product->product_custom_data = copyData(customData);
    new_product->product_custom_data_copy_function = copyData;
    new_product->product_free_custom_data_function = freeData;
    new_product->product_prodPrice = prodPrice;
    new_product->productCopy = productCopy;
    new_product->productFree = productFree;
    new_product->productCompare = productCompare;

    AmountSetResult new_product_register_result = asRegister(matamikya->warehouse_products, new_product);
    if(new_product_register_result == AS_ITEM_ALREADY_EXISTS)
    {
        new_product->productFree(new_product);
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    AmountSetResult new_product_amount_change_result = asChangeAmount(matamikya->warehouse_products,
                                                                     new_product, amount);
    if(new_product_amount_change_result == AS_INSUFFICIENT_AMOUNT) //this means that amount is less than 0
    {
        new_product->productFree(new_product);
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }
    productFree(new_product); //need to be free'd because asRegister makes a copy of it
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount)
{
    if(matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    double current_product_current_amount = 0;
    Product current_product = getProductById(matamikya->warehouse_products, id);
    if((current_product == NULL) || (asGetAmount(matamikya->warehouse_products,
                                                 current_product, &current_product_current_amount) != AS_SUCCESS))
    {
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    if(!(checkIfAmountTypeIsConsistent(amount, current_product->product_amount_type)))//check amount to be added
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }
    if(!(checkIfAmountTypeIsConsistent((amount + current_product_current_amount),
                                         current_product->product_amount_type))) //check amount after addition
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }
    AmountSetResult change_result = asChangeAmount(matamikya->warehouse_products, current_product, amount);
    if(change_result == AS_INSUFFICIENT_AMOUNT)
    {
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }
    return MATAMIKYA_SUCCESS;
}

//clear a product from a Matamikya warehouse.
MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
    if(matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Product product_to_delete = getProductById(matamikya->warehouse_products, id);
    AS_FOREACH(Order, orders_iterator, matamikya->warehouse_orders)
    {
        AS_FOREACH(Product, products_iterator, orders_iterator->order_items)
        {
            if (!productCompare(product_to_delete, products_iterator)) {
                asDelete(orders_iterator->order_items, products_iterator);
                break;
            }
        }
    }
    AmountSetResult delete_res = asDelete(matamikya->warehouse_products, product_to_delete);
    if((delete_res == AS_ITEM_DOES_NOT_EXIST) || delete_res == AS_NULL_ARGUMENT)
    {
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    return MATAMIKYA_SUCCESS;
}

//create a new empty order in a Matamikya warehouse, and return the order's id.
unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
    if (matamikya==NULL){
        return FAILURE;
    }
    Order new_order=malloc(sizeof(*new_order));
    if (new_order==NULL){
        return FAILURE;
    }
    unsigned int new_id=generateNewId(matamikya);
    new_order->order_id=new_id;
    new_order->orderCopy=orderCopy;
    new_order->orderDestroy=orderDestroy;
    new_order->orderCompare=orderCompare;
    new_order->order_items=asCreate(productCopy, productFree, productCompare);
    asRegister(matamikya->warehouse_orders, new_order);
    orderDestroy(new_order);
    return new_id;
}

//add/increase/remove/decrease products to an existing order.
MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
                                     const unsigned int productId, const double amount)
{
    if (matamikya==NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order current_order=getOrderById(matamikya->warehouse_orders, orderId);
    if (current_order==NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }

    Product product_in_warehouse=getProductById(matamikya->warehouse_products, productId);
    if (product_in_warehouse==NULL){
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    if(!(checkIfAmountTypeIsConsistent(amount, product_in_warehouse->product_amount_type)))//check amount to be added
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }
    if(amount==0) {
        return MATAMIKYA_SUCCESS;
    }
    Product product_in_order=getProductById(current_order->order_items, productId);
    //if the item does not already exist in this order, it needs to be added
    if (product_in_order==NULL) {   
        if (amount>0) {
            asRegister(current_order->order_items, product_in_warehouse);
            asChangeAmount(current_order->order_items, product_in_warehouse, amount);
            return MATAMIKYA_SUCCESS;
        }
        else { //amount<=0
            return MATAMIKYA_SUCCESS;
        }
    }
    //if the item exists in the order
    else {
        double current_amount=0;
        asGetAmount(current_order->order_items, product_in_order, &current_amount);
        if ((current_amount + amount) <= 0) {
            asDelete(current_order->order_items, product_in_order);
            return MATAMIKYA_SUCCESS;
        }
        else {
            asChangeAmount(current_order->order_items, product_in_order, current_amount + amount);
            return MATAMIKYA_SUCCESS;
        }
    }
}

//ship an order and remove it from a Matamikya warehouse.
MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId)
{
    if (matamikya==NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order current_order=getOrderById(matamikya->warehouse_orders, orderId);
    if (current_order==NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    if (checkInventory(matamikya->warehouse_products, current_order, orderId)){
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    } 
    AS_FOREACH(Product, order_iterator, current_order->order_items){  //iterate on order's items
        AS_FOREACH(Product, iterator, matamikya->warehouse_products){ //iterate on inventory's items
            if (!(iterator->productCompare(iterator, order_iterator))){
                double out_amount=0;
                asGetAmount(current_order->order_items, order_iterator, &out_amount);
                shipItem(matamikya, iterator, out_amount);
            }
        }
    }
    mtmCancelOrder(matamikya, orderId);
    return MATAMIKYA_SUCCESS;
}

//cancel an order and remove it from a Matamikya warehouse.
MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId)
{
    if (matamikya==NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order current_order=getOrderById(matamikya->warehouse_orders, orderId);
    if (current_order==NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    asDelete(matamikya->warehouse_orders, current_order);
    return MATAMIKYA_SUCCESS;
}

//print a Matamikya warehouse and its contents
MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output)
{
    if (matamikya == NULL || output == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    fprintf(output, "Inventory Status:\n");
    AS_FOREACH(Product, iterator, matamikya->warehouse_products) {
        double current_amount=0;
        asGetAmount(matamikya->warehouse_products, iterator, &current_amount);
        mtmPrintProductDetails(iterator->product_name, iterator->product_id,
                               current_amount, iterator->product_prodPrice(iterator->product_custom_data, 1), output);
    }
    return MATAMIKYA_SUCCESS;
}

//print a summary of an order from a Matamikya warehouse
MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output)
{
    if (matamikya == NULL || output == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order current_order=getOrderById(matamikya->warehouse_orders, orderId);
    if (current_order==NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    mtmPrintOrderHeading(orderId, output);
    AS_FOREACH(Product, iterator, current_order->order_items) {
        double current_amount=0;
        asGetAmount(current_order->order_items, iterator, &current_amount);
        mtmPrintProductDetails(iterator->product_name, iterator->product_id, current_amount,
                             iterator->product_prodPrice(iterator->product_custom_data, current_amount), output);
    }
    double total_price=calculateTotalPrice(current_order);
    mtmPrintOrderSummary(total_price, output);
    return MATAMIKYA_SUCCESS;
}

//print the best selling products of a Matamikya
MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output)
{
    if (matamikya == NULL || output == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    fprintf(output, "Best Selling Product:\n");
    Product best_seller = getBestSeller(matamikya);
    double temp_income = best_seller->product_income;
    if (temp_income == 0){
        fprintf(output, "none\n");
        return MATAMIKYA_SUCCESS;
    }
    mtmPrintIncomeLine(best_seller->product_name, best_seller->product_id,
                         best_seller->product_income , output);
    return MATAMIKYA_SUCCESS;
}
            //******//
