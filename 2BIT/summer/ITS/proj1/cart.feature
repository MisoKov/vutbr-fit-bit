Feature: Cart management

  Scenario: Add featured to cart
    Given the web browser is at home page of store
    When user clicks Add to cart under "iPhone" product
    Then "iPhone" product is in cart
    And cart button is showing price "$101.00"

  Scenario: Removing item from cart window
    Given web browser is at main page
    And product "iPhone" is in cart
    When product "iPhone" is deleted from cart window using red X button
    Then cart is displaying 0 items in it
    And cart button is showing price "$0.00"

  Scenario: Removing item from cart page
    Given Given web browser is at shopping cart page
    And "iPhone" product is in cart
    When user deletes "iPhone" form cart
    Then cart is empty

  Scenario: Add to cart from product page
    Given web browser is at page of "iPhone" product
    When product is added to cart
    Then "iPhone" product is in cart
    And cart button is showing price "$101.00"

  Scenario: Checking shipping price
    Given web browser is at shopping cart page
    And "iPhone" product is in cart
    When user enters shipping destination into shipping estimation tab
    Then message box with shipping methods and prices is displayed

  Scenario: Updating quantity from cart
    Given web browser is at shopping cart page
    And "iPhone" product is in cart
    When user updates quantity to "3"
    Then quantity of product is "3"
    And total price is "$303.00"

  Scenario: Moving to checkout from cart page
    Given web browser is at shopping cart page
    And "iPhone" product is in cart
    When user clicks Checkout button
    Then the checkout page is displayed

  Scenario: Moving to checkout from cart window
    Given web browser is at main page
    And product "iPhone" is in cart
    When user clicks on checkout in cart window
    Then the checkout page is displayed

  Scenario: Add to cart from category page
    Given web browser is at page displaying "Cameras" category
    When user adds product "Nikon D300" to cart
    Then "Nikon D300" product is in cart
    And cart button is showing price "$80.00"