from behave import *
import selenium
from selenium import webdriver
from selenium.webdriver.common.by import By
from help_functions import find_element

# Common steps for all three features
@given(u'the web browser is at home page of store')
def home_page_load(context):
    context.driver.get('http://mys01.fit.vutbr.cz:8051/')
    assert context.driver.title == "Your Store", "Browser did not load home page"

@then(u'"{product}" product is in cart')
def check_product_in_cart_win(context, product):
    cart_butt = find_element(context.driver, By.ID, "cart-total")
    if not cart_butt:
        cart_butt = find_element(context.driver, By.XPATH, "//div[@id='cart']/button")
    assert cart_butt, "Cart button not found"
    cart_butt.click()
    cart_content = find_element(context.driver, By.XPATH, "//a[contains(text(),'"+product+"')]/ancestor::td[@class='text-left']")
    assert cart_content, "Product not found in cart"
    assert cart_content.text == product, "Different product found"

@given(u'"{product}" product is in cart')
def setup_cart_add_product(context, product):
    """
    Only featured products from main page can be added to cart in setup phase since the search feature doesnt work
    """
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=common/home")
    context.driver.find_element(By.XPATH, "//h4/a[text() = \""+product+"\"]/ancestor::div[2]/div[3]/button").click()
    price = find_element(context.driver, By.XPATH, "//h4/a[text()= \"" + product + "\"]/ancestor::div[1]/p[2]")
    assert price, "Price of product not found"
    i = price.text.find("\n")
    context.price = price.text[:i]