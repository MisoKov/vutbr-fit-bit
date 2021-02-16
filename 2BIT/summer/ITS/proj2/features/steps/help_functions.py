import selenium
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.common.exceptions import NoSuchElementException

def find_element(driver, how, what):
    """
    Tries to find element on page
    :param driver: webdriver
    :param how: searching method
    :param what: search expression
    :return: True on success, False when not found
    """
    try:
        element = driver.find_element(by=how, value=what)
    except NoSuchElementException as e:
        return False
    else:
        return element

def comparison_clear(driver):
    """
    Removes first item from comparison page
    :param driver: webdriver
    """
    remove_button = find_element(driver, By.LINK_TEXT, "Remove")
    remove_button.click()

def cart_clean(driver):
    """
    Removes all items from cart
    :param driver: webdriver
    """
    driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/cart")
    butt_list = driver.find_elements(By.CSS_SELECTOR, ".fa-times-circle")
    num = len(butt_list)
    for i in range(num):
        driver.find_element(By.CSS_SELECTOR, ".fa-times-circle").click()