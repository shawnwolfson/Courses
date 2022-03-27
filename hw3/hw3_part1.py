NAME_INDEX = 2
SALES_INDEX = 2
PRICE_INDEX = 3
AMOUNT_INDEX = 4
SHIP_OPERATOR_INDEX = 4
SHIP_ORDER_PHRASE_INDEX = 9


def addOperation(inventory_dict, name, price, amount):
    sales = 0
    if (price >= 0) and (amount >= 0) and (not name in inventory_dict.keys()):
        inventory_dict[name] = [price, amount, sales]

def changeOperation(inventory_dict, name, amount):
    if name in inventory_dict.keys():
        inventory_dict[name][1] += amount

def shipOperation(inventory_dict, row):
    row[0] = row[0][SHIP_ORDER_PHRASE_INDEX::1] #remove "ship order" phrase from first element
    for e in row:
        temp = e.split(',')
        name = temp[0]
        amount = float(temp[1])
        if (not name in inventory_dict.keys()) or (amount <= 0) or ((inventory_dict[name][1] - amount) < 0):
            continue
        inventory_dict[name][1] -= amount
        inventory_dict[name][SALES_INDEX] += inventory_dict[name][0] * amount

def sortFunction(e):
    return e[1]

def processInventory(content):
    inventory_dict = {}
    for row in content:
        operation = row[0:SHIP_OPERATOR_INDEX]
        if operation == "ship":
            row_for_shipment = row.replace(' ', '').split("--")
            shipOperation(inventory_dict, row_for_shipment)
        else:
            row = row.split(' ')
            operation = row[0]
            name = row[NAME_INDEX]
            price = float(row[PRICE_INDEX]) 
            if operation == "add":
                amount = float(row[AMOUNT_INDEX])
                addOperation(inventory_dict, name, price, amount)
            if operation == "change":
                change_amount = float(row[AMOUNT_INDEX-1])
                changeOperation(inventory_dict, name, change_amount) 
    return inventory_dict




def find_best_selling_product(file_name):
    best_seller = ('',0)
    file = open(file_name, 'r')
    content = file.readlines()
    file.close
    if len(content) == 0:
        return best_seller
    inventory = processInventory(content)
    if len(inventory) == 0:
        return best_seller

    best_seller = ['',0]
    for key, value in inventory.items():
        if value[SALES_INDEX] > best_seller[1]:
            best_seller[0] = key
            best_seller[1] = value[SALES_INDEX]
    for key, value in inventory.items():
        if (value[SALES_INDEX] == best_seller[1]) and (key < best_seller[0]):
            best_seller[0] = key
    return (best_seller[0], best_seller[1])


def find_k_most_expensive_products(file_name, k):
    file = open(file_name, 'r')
    content = file.readlines()
    file.close
    if (k <= 0) or (len(content) == 0):
        return []

    inventory = processInventory(content)
    inventory_list = []
    if k > len(inventory):
        k = len(inventory)
    for key, value in inventory.items():
        inventory_list.append([key,value[0]])
    inventory_list.sort(reverse = True, key = sortFunction)
    counter = 1
    top_k_prices = []
    current_price = inventory_list[0][1]
    top_k_prices.append(current_price)
    for el in inventory_list:
        if el[1] < current_price and counter < k:
            current_price = el[1]
            top_k_prices.append(current_price)
            counter += 1
    return_list = []
    for e in top_k_prices:
        temp_list = []
        for el in inventory_list:
            name = el[0]
            price = el[1]
            if price == e:
                temp_list.append(name)
        temp_list.sort()
        return_list += temp_list
    return return_list[0:k]