from behave import *
import selenium
from selenium import webdriver
from selenium.webdriver.common.by import By
from help_functions import find_element
from help_functions import comparison_clear

@when(u'the user selects "{product}" for comparison from home page')
def add_featured_to_comparison(context, product):
    comp_butt = find_element(context.driver, By.XPATH, "//h4/a[text() =\""+product+"\"]/ancestor::div[2]/div[3]/button[3]")
    assert comp_butt, "Compare button for required product not found"
    comp_butt.click()

@when(u'the user clicks \'product comparison\'')
def go_to_comparison_page(context):
    comparison_link = find_element(context.driver, By.LINK_TEXT, "product comparison")
    assert comparison_link, "Comparison link not found"
    comparison_link.click()

@then(u'result page comparing "{product1}" and "{product2}" is displayed')
def check_comparison_of_2(context, product1, product2):
    assert context.driver.title == "Product Comparison"
    product1_window = find_element(context.driver, By.XPATH, "//strong[contains(.,'"+product1+"')]")
    assert product1_window, "Product1 not found in comparison"
    product2_window = find_element(context.driver, By.XPATH, "//strong[contains(.,'"+product2+"')]")
    assert product2_window, "Product2 not found in comparison"

    # teardown
    comparison_clear(context.driver)
    comparison_clear(context.driver)

@given(u'web browser is at \'MP3 players\' category page')
def load_mp3_page(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/category&path=34")
    assert context.driver.title == "MP3 Players", "Not on MP3 Players page"

@when(u'the user selects "{product}" for comparison from category page') # x2
def add_featured_to_comparison(context, product):
    comp_butt = find_element(context.driver, By.XPATH, "//h4/a[text() = \""+product+"\"]/ancestor::div[3]/div[2]/div[2]/button[3]")
    assert comp_butt, "Compare button for required product not found"
    comp_butt.click()

@then(u'result page comparing "{product}" is displayed')
def check_product_in_comp(context, product):
    product_window = find_element(context.driver, By.XPATH, "//strong[contains(.,'"+product+"')]")
    assert product_window, "Product not found in comparison"

    # teardown
    comparison_clear(context.driver)

@given(u'the web browser is at product page of \'iPhone\'')
def load_iphone_page(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/product&product_id=40")
    assert context.driver.title == "iPhone"

@when(u'the user adds "{product}" to comparison')
def add_to_comp_from_product(context, product):
    comp_butt = find_element(context.driver, By.CSS_SELECTOR, "button.btn - default: nth - child(2)")
    assert comp_butt, "Compare button not found"
    comp_butt.click()

@given(u'the web browser is at page comparing "{product1}" and "{product2}"')
def create_comp_of_two(context, product1, product2):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=common/home")
    find_element(context.driver, By.XPATH, "//h4/a[text() =\"" + product1 + "\"]/ancestor::div[2]/div[3]/button[3]").click()
    find_element(context.driver, By.XPATH, "//h4/a[text() =\"" + product2 + "\"]/ancestor::div[2]/div[3]/button[3]").click()
    find_element(context.driver, By.LINK_TEXT, "product comparison").click()

@when(u'add to cart button is clicked under "{product}" product')
def add_to_cart_from_comp(context, product):
    products_in_comp = context.driver.find_elements(By.XPATH, "//a/strong")
    product_list = []
    for e in products_in_comp:
        product_list.append(e.text)
    try:
        index = product_list.index(product)
    except ValueError:
        assert False, "Product not in comparison table"
    add_buttons = context.driver.find_elements(By.XPATH, "//input[@value='Add to Cart']")
    assert add_buttons[index], "Add to cart button not found"
    add_buttons[index].click()

@when(u'user removes "{product}" from comparison')
def remove_from_comp(context, product):
    products_in_comp = context.driver.find_elements(By.XPATH, "//a/strong")
    product_list = []
    for e in products_in_comp:
        product_list.append(e.text)
    try:
        index = product_list.index(product)
    except ValueError:
        assert False, "Product not in comparison table"
    remove_buttons = context.driver.find_elements(By.LINK_TEXT, "Remove")
    assert remove_buttons[index], "Remove button not found"
    remove_buttons[index].click()
    context.removed = product

@then(u'comparison is displaying only "{product}"')
def check_comp_only_one_product(context, product):
    products_in_comp = context.driver.find_elements(By.XPATH, "//a/strong")
    product_list = []
    for e in products_in_comp:
        product_list.append(e.text)
    assert len(product_list) > 0, "Comparison should still contain other product"
    assert not context.removed in product_list, "Comparison still contains the product"

    #teardown
    comparison_clear(context.driver)

@when(u"the user adds product to comparison from it's page")
def add_featured_to_comparison(context):
    comp_butt = find_element(context.driver, By.CSS_SELECTOR, ".fa-exchange")
    assert comp_butt, "Compare button not found"
    comp_butt.click()