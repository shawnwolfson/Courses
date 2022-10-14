#Function to check if a string is a palindrome.
def is_palindrome(str):
    reversed_string = ''.join(reversed(str))
    return (reversed_string == str)

#Function to get all substrings of a given length of a string.
def get_all_substrings_in_a_given_length(str, length):
    return [str[start_index: start_index + length] for start_index in range(len(str) - length + 1)]

#Function to set the values that will go into the dictionary.
def get_dict_values(str, length):
    substrings = get_all_substrings_in_a_given_length(str, length)
    values = []
    for element in substrings:
        if(is_palindrome(element)):
          values.append(element)
    return values

#The requested function.
def get_palindrom_dict(str):
    new_dict = {}
    if (str == ""):
        return new_dict
    
    for t in range(1,len(str) + 1):
       values = get_dict_values(str, t)
       if(values):
            new_dict[t] = values
    return new_dict




#Function to split the given strings into a list of 2 strings as needed.
def get_odd_and_even_strings(str):
    new_strings = []
    new_strings.append(str[::2])
    new_strings.append(str[1::2])
    return new_strings

#Function to check is the two strings that the list contains are similiar by the definition given.
def check_if_similar(strings_to_check):
    iteratable_string = strings_to_check[0]
    static_string = strings_to_check[1]
    #This dictionary maps the changes of the letters.
    switched_paires_dict = {}
    for index in range(len(static_string)):
        if(iteratable_string[index] in switched_paires_dict):
            if(switched_paires_dict[iteratable_string[index]] != static_string[index]):
                return False
        switched_paires_dict[iteratable_string[index]] = static_string[index]
    return True


#The requested function
def check_match(str):
    if(str == ""):
        return True
    strings_to_check = get_odd_and_even_strings(str)
    if(len(strings_to_check[0]) != len(strings_to_check[1])):
        return False
    return (check_if_similar(strings_to_check))
