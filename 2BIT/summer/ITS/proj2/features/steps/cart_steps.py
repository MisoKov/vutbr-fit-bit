from behave import *
import selenium
from selenium import webdriver
from selenium.webdriver.common.by import By
from help_functions import find_element
from help_functions import cart_clean

@when(u'user clicks Add to cart under "{product}" product on home page')
def add_to_cart_from_home(context, product):
    add_butt = find_element(context.driver, By.XPATH, "//h4/a[text() = \""+product+"\"]/ancestor::div[2]/div[3]/button")
    assert add_butt, "Add to cart button for required product not found"
    price = find_element(context.driver, By.XPATH, "//h4/a[text()= \"" + product + "\"]/ancestor::div[1]/p[2]")
    assert price, "Price of product not found"
    i = price.text.find("\n")
    context.price = price.text[:i]
    add_butt.click()

@when(u'user clicks Add to cart under "{product}" product on category page')
def add_to_cart_from_category(context, product):
    add_butt = find_element(context.driver, By.XPATH, "//h4/a[text() = \""+product+"\"]/ancestor::div[3]/div[2]/div[2]/button")
    assert add_butt, "Add to cart button for required product not found"
    add_butt.click()

@then(u'cart button is showing correct price')
def check_correct_price_in_cart_button(context):
    cart_butt = find_element(context.driver, By.ID, "cart-total")
    cart_butt = False
    if not cart_butt:
        cart_butt = find_element(context.driver, By.XPATH, "//div[@id='cart']/button")
    assert cart_butt, "Cart button not found"
    i = cart_butt.text.find('-')
    cart_price = cart_butt.text[i+2:]
    assert context.price == cart_price, "Price is not correct"

    #teardown
    cart_clean(context.driver)

@then(u'cart button is showing price "{price}"')
def check_equal_price_in_cart_button(context, price):
    cart_butt = find_element(context.driver, By.ID, "cart-total")
    assert cart_butt, "Cart button not found"
    i = cart_butt.text.find('-')
    cart_price = cart_butt.text[i+2:]
    assert price == cart_price, "Price is not equal"

    #teardown
    cart_clean(context.driver)

@when(u'product "{product}" is deleted from cart window using red X button')
def delete_cart_by_X_window(context, product):
    cart_butt = find_element(context.driver, By.ID, "cart-total")
    assert cart_butt, "Cart button not found"
    cart_butt.click()
    x_butt = find_element(context.driver, By.CSS_SELECTOR, ".btn-danger")
    assert x_butt, "Delete button not found"
    x_butt.click()

@then(u'cart is displaying 0 items in it')
def check_empty_cart_window(context):
    cart_butt = find_element(context.driver, By.ID, "cart-total")
    assert cart_butt, "Cart button not found"
    assert "0 item(s) - $0.00" in cart_butt.text, "Cart button is not displaying empty cart"
    cart_butt.click()
    cart_text = find_element(context.driver, By.XPATH, "//div[@id='cart']/ul/li/p")
    assert cart_text, "Text inside cart window not found"
    assert cart_text.text == "Your shopping cart is empty!", "Cart window not displaying proper message for empty cart"


@given(u'web browser is at shopping cart page')
def load_cart_page(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/cart")
    assert context.driver.title == "Shopping Cart", "Not Shopping Cart page"

@when(u'user deletes "{product}" form cart')
def delete_product_from_cart_page(context, product):
    delete_butt = find_element(context.driver, By.CSS_SELECTOR, ".fa-times-circle")
    assert delete_butt, "Delete button not found"
    delete_butt.click()

@then(u'cart is empty')
def check_empty_cart_page(context):
    assert "Your shopping cart is empty!" in context.driver.page_source, "Cart is not empty"


@given(u'web browser is at page of "{product}" product')
def load_product_page(context, product):
    """
    Works only for featured products on home page
    """
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=common/home")
    context.driver.find_element_by_link_text(product).click()
    assert context.driver.title == product, "Not on product page"

@when(u'product is added to cart from it\'s page')
def add_to_cart_from_product_page(context):
    add_butt = find_element(context.driver, By.ID, "button-cart")
    assert add_butt, "Add to cart button not found"
    price = find_element(context.driver, By.CSS_SELECTOR, "h2:nth-child(1)")
    assert price, "Price not found"
    context.price = price.text
    add_butt.click()

@when(u'user enters shipping destination into shipping estimation tab')
def submit_estimation_tab(context):
    estimation_tab = find_element(context.driver, By.LINK_TEXT, "Estimate Shipping & Taxes")
    assert estimation_tab, "Estimation tab not found"
    estimation_tab.click()
    in_country = find_element(context.driver, By.ID, "input-country")
    assert in_country, "Country input field not found"
    in_country.click()
    in_country.find_element(By.XPATH, "//option[. = 'Tonga']").click()
    in_region = find_element(context.driver, By.ID, "input-zone")
    assert in_region, "Region input field not found"
    in_region.click()
    in_region.find_element(By.XPATH, "//option[. = 'Tongatapu']").click()
    in_postcode = find_element(context.driver, By.ID, "input-postcode")
    assert in_postcode, "Postcode input field not found"
    in_postcode.click()
    in_postcode.send_keys("00042")
    quotes_butt = find_element(context.driver, By.ID, "button-quote")
    assert quotes_butt, "Button 'Get Quotes' not found"
    quotes_butt.click()

@then(u'message box with shipping methods and prices is displayed')
def check_extimation(context):
    method = find_element(context.driver, By.NAME, "shipping_method")
    assert method, "No method found"
    method.click()
    apply_butt = find_element(context.driver, By.ID, "button-shipping")
    assert apply_butt, "No Apply shipping button found"
    apply_butt.click()
    assert "Success: Your shipping estimate has been applied!" in context.driver.page_source, "No success message found"
    rate_colon = find_element(context.driver, By.XPATH, "//strong[contains(.,'Flat Shipping Rate:')]")
    assert rate_colon, "Shipping Rate information not found"

@when(u'user updates quantity to "{num}"')
def change_quantity(context, num):
    q_field = find_element(context.driver, By.XPATH, "//td[4]/div/input")
    assert q_field, "Quantity input field not found"
    q_field.click()
    q_field.clear()
    q_field.send_keys(num)
    change_q_butt = find_element(context.driver, By.XPATH, "//button[@type='submit']")
    assert change_q_butt, "Quantity submit button not found"
    change_q_butt.click()

@then(u'quantity of product is "{num}"')
def check_quantity(context, num):
    q_field = find_element(context.driver, By.XPATH, "//td[4]/div/input")
    assert q_field, "Quantity input field not found"
    assert q_field.get_attribute("value") == num, "Incorrect quantity"


@then(u'total price is "{num}" times the previous total')
def check_total_price(context, num):
    expected_price = float(context.price[1:])
    expected_price *= 3
    expected_price = "$"+str(expected_price)
    if len(expected_price[expected_price.find(".")+1:]) < 2:
        expected_price = expected_price + "0"
    total_price = find_element(context.driver, By.CSS_SELECTOR, "tbody .text-right:nth-child(6)")
    assert total_price, "Not found total price field in table"
    assert total_price.text == expected_price, "Table price is not correct after change of quantity"

@when(u'user clicks Checkout button')
def to_checkout_from_topper_button(context):
    checkout_butt = find_element(context.driver, By.CSS_SELECTOR, ".pull-right > .btn")
    assert checkout_butt, "No Checkou button found"
    checkout_butt.click()

@then(u'the checkout page is displayed')
def check_checkout_page(context):
    assert context.driver.title == "Checkout", "Not on Checkout page"
    header = find_element(context.driver, By.CSS_SELECTOR, "h1")
    assert header, "Checkout header not found"
    assert header.text == "Checkout", "Incorrect header found"

@when(u'user clicks on checkout in cart window')
def to_checkout_from_cart_window(context):
    cart_butt = find_element(context.driver, By.XPATH, "//div[@id='cart']/button")
    assert cart_butt, "Cart button not found"
    cart_butt.click()
    check_butt = find_element(context.driver, By.XPATH, "//strong[contains(.,' Checkout')]")
    assert check_butt, "Checkout button in cart window not found"
    check_butt.click()

@given(u'web browser is at page displaying \'Cameras\' category')
def load_cameras_page(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/category&path=33")
    assert context.driver.title == "Cameras"

@when(u'user adds product "{product}" to cart from category page')
def add_to_cart_from_category_page(context, product):
    add_butt = find_element(context.driver, By.XPATH, "//a[contains(text(),'"+product+"')]/ancestor::div[2]/div[2]/button")
    assert add_butt, "Add button for product not found"
    price = find_element(context.driver, By.XPATH, "//a[contains(text(),'"+product+"')]/ancestor::div[1]/p[2]")
    assert price, "Price of product not found"
    i = price.text.find("\n")
    context.price = price.text[:i]
    add_butt.click()
