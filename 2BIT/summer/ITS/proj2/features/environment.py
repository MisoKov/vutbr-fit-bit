from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait

def before_scenario(context, scenario):
    dp = {'browserName': 'firefox', 'marionette': 'true', 'javascriptEnabled': 'true'}
    context.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub', desired_capabilities=dp)
    #context.driver = webdriver.Firefox() # --- local testing
    context.driver.implicitly_wait(15)
    context.wait = WebDriverWait(context.driver, 15)

def after_scenario(context, scenario):
    context.driver.quit()

