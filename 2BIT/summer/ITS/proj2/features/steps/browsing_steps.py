from behave import *
import selenium
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.common.exceptions import NoSuchElementException
from help_functions import find_element

@when(u'the user enters "{input}" to the search bar')
def search_bar_filled(context, input):
    bar = find_element(context.driver, By.NAME, "search")
    assert bar, "No search bar text field found"
    bar.clear()
    bar.send_keys(input)
    search_icon = find_element(context.driver, By.CSS_SELECTOR, ".btn-default")
    assert search_icon, "No search button found"
    search_icon.click()

@then(u'products related to the "{input}" are shown on result page')
def check_search_results(context, input):
    assert context.driver.title != "404 Not Found", "Result page failed to load"
    assert input in context.driver.page_source or "0 results" in context.driver.page_source, "No search results found"

@when(u'user changes currency to "{currency}"')
def currency_change(context, currency):
    driver = context.driver
    drop_down_menu = find_element(driver, By.CSS_SELECTOR, ".fa-caret-down")
    assert drop_down_menu, "Currency menu not found"
    drop_down_menu.click()
    if currency == "euro":
        context.curr = "EUR"
    elif currency == "dollar":
        context.curr = "USD"
    elif currency == "pound":
        context.curr = "GBP"
    else:
        raise AssertionError("Unsupported currency")
    curr_button = find_element(driver, By.NAME, context.curr)
    assert curr_button, "Currency button not found"
    curr_button.click()

@then(u'currency at cart window will be "{currency}"')
def check_cart_window_currency(context, currency):
    if currency == "euro":
        if context.curr == "EUR":
            context.symbol = "€"
        else:
            raise AssertionError("Different currency passed from previous steps")
    elif currency == "dollar":
        if context.curr == "USD":
            context.symbol = "$"
        else:
            raise AssertionError("Different currency passed from previous steps")
    elif currency == "pound":
        if context.curr == "GBP":
            context.symbol = "£"
        else:
            raise AssertionError("Different currency passed from previous steps")
    else:
        raise AssertionError("Unsupported currency")
    driver = context.driver
    cart = find_element(driver, By.ID, "cart-total")
    assert cart, "Cart window not found"
    assert context.symbol in cart.text, "Not matching currency in cart window"

@then(u'currency of prices of featured products will be "{currency}"')
def check_featured_products_currency(context, currency):
    driver = context.driver
    if currency == "euro":
        if context.curr != "EUR":
            raise AssertionError("Different currency passed from previous steps")
    elif currency == "dollar":
        if context.curr != "USD":
            raise AssertionError("Different currency passed from previous steps")
    elif currency == "pound":
        if context.curr != "GBP":
            raise AssertionError("Different currency passed from previous steps")
    else:
        raise AssertionError("Unsupported currency")
    try:
        elements = driver.find_elements(By.CLASS_NAME, "price")
    except NoSuchElementException as e:
        raise NoSuchElementException("No price found")
    else:
        for e in elements:
            assert context.symbol in e.text, "Incorrect currency in price of featured product"
    try:
        elements = driver.find_elements(By.CLASS_NAME, "price-tax")
    except NoSuchElementException as e:
        raise NoSuchElementException("No ex-tax price found")
    else:
        for e in elements:
            assert context.symbol in e.text, "Incorrect currency in price without tax of featured product"

    # teardown
    context.driver.find_element(By.CSS_SELECTOR, ".fa-caret-down").click()
    context.driver.find_element(By.NAME, "USD")

@when(u'user chooses "{category}" category from menu')
def click_on_category(context, category):
    context.cat = category
    cat_button = find_element(context.driver, By.LINK_TEXT, category)
    assert cat_button, "Category button not found"
    cat_button.click()

@then(u'page containing "{category}" products is displayed')
def check_category_page(context, category):
    assert context.cat == category, "Not same category as in previous steps"
    assert context.driver.title == category, "Not on required category page"
    header = find_element(context.driver, By.TAG_NAME, "h2")
    assert header, "Content header not found"
    assert header.text == category, "Not displaying required category"

@when(u'users clicks \'Brands\' in footer')
def click_brands(context):
    brands_butt = find_element(context.driver, By.LINK_TEXT, "Brands")
    assert brands_butt, "Brands button not found"
    brands_butt.click()

@then(u'page showing all brands is displayed')
def check_brands_page(context):
    assert context.driver.title == "Find Your Favorite Brand", "Not on brands selection page"
    h1 = find_element(context.driver, By.CSS_SELECTOR, "h1")
    assert h1, "Brands header not found"
    assert h1.text == "Find Your Favorite Brand", "Incorrect header"
    brand_loc_menu = find_element(context.driver, By.LINK_TEXT, "Brand")
    assert brand_loc_menu, "Brand not found in breadcrumb link menu"

@given(u'the web browser is at Brands page')
def load_brands_page(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/manufacturer")
    assert context.driver.title == "Find Your Favorite Brand"

@when(u'users chooses "{brand}" brand')
def click_on_brand(context, brand):
    brand_link = find_element(context.driver, By.LINK_TEXT, brand)
    assert brand_link, "Brand not found"
    brand_link.click()

@then(u'page showing all "{brand}" products is displayed')
def check_brand_page(context, brand):
    assert context.driver.title == brand, "Not on required brand page"
    header = find_element(context.driver, By.TAG_NAME, "h2")
    assert header, "Header not found"
    assert header.text == brand, "Not displaying required brand header"
    brand_loc_menu = find_element(context.driver, By.LINK_TEXT, brand)
    assert brand_loc_menu, "Required brand not found in breadcrumb link menu"

@given(u'web browser is at \'Contact us\' page')
def load_contact_us_page(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=information/contact")
    assert context.driver.title == "Contact Us"

@when(u'user correctly submits contact form')
def submit_form(context):
    name_field = find_element(context.driver, By.ID, "input-name")
    assert name_field, "Name input field not found"
    name_field.click()
    name_field.send_keys("Tester Person")
    email_field = find_element(context.driver, By.ID, "input-email")
    assert email_field, "Email input field not found"
    email_field.click()
    email_field.send_keys("testing@email.com")
    enquiry_field = find_element(context.driver, By.ID, "input-enquiry")
    enquiry_field.click()
    enquiry_field.send_keys("Some enquiry to test")
    submit_butt = find_element(context.driver, By.CSS_SELECTOR, ".btn-primary")
    assert submit_butt, "Submit button not found"
    submit_butt.click()

@then(u'page showing success is displayed')
def check_form_success(context):
    assert "Your enquiry has been successfully sent to the store owner!" in context.driver.page_source, \
        "Submitting the contact form was not successfull"

@given(u'web browser is at \'Desktop\' category page')
def load_desktops_page(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/category&path=20")
    assert context.driver.title == "Desktops", "Not on Desktops page"

@when(u'user clicks logo of the store')
def click_logo(context):
    logo = find_element(context.driver, By.CSS_SELECTOR, "#logo .img-responsive")
    assert logo, "Logo not found"
    logo.click()

@then(u'home page of store is displayed')
def check_home_page(context):
    assert context.driver.title == "Your Store", "Not on Home page"
    featured = find_element(context.driver, By.CSS_SELECTOR, "h3")
    assert featured, "No header on home page found"
    assert featured.text == "Featured", "No 'Featured' header found on home page"

@given(u'web browser is at \'Phones & PDAs\' category page')
def load_phones_pda_page(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/category&path=24&sort=pd.name&order=ASC")
    assert context.driver.title == "Phones & PDAs", "Not on Phones & PDAs page"

@when(u'user chooses sorting by "{order}"')
def choose_sort_by(context, order):
    assert order != "ascending name" or \
           order != "descending name" or \
           order != "ascending price" or \
           order != "descending price", "Unsupported sorting order selected"
    context.sorting = order
    if order == "ascending price":
        dropdown_option = "Price (Low > High)"
    elif order == "descending price":
        dropdown_option = "Price (High > Low)"
    elif order == "ascending name":
        dropdown_option = "Name (A - Z)"
    elif order == "descending name":
        dropdown_option = "Name (Z - A)"
    dropdown = find_element(context.driver, By.ID, "input-sort")
    assert dropdown, "Sorting dropdown menu not found"
    dropdown.click()
    opt = find_element(context.driver, By.XPATH, "//option[. = '"+dropdown_option+"']")
    assert opt, "Sorting option not found in menu"
    opt.click()

@then(u'products are sorted by "{order}"')
def check_sorted_items(context, order):
    assert order == context.sorting, "Not same order as in previous steps"
    if "ascending" in order:
        sort_order = "asc"
    else:
        sort_order = "desc"
    import time
    time.sleep(5)
    if "name" in order:
        elements = context.driver.find_elements(By.TAG_NAME, "h4")
        name_list = []
        for e in elements:
            name_list.append(e.text.lower())
        if sort_order == "asc":
            assert sorted(name_list) == name_list, "Not sorted by name in ascending order"
        else:
            assert sorted(name_list, reverse=True) == name_list, "Not sorted by name in descending order"
    else:
        elements = context.driver.find_elements(By.CLASS_NAME, "price")
        price_list = []
        for e in elements:
            i = e.text.find("\n")
            price_list.append(float(e.text[1:i]))
        if sort_order == "asc":
            assert sorted(price_list) == price_list, "Not sorted by price in ascending order"
        else:
            assert sorted(price_list, reverse=True) == price_list, "Not sorted by price in descending order"
