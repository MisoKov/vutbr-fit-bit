Feature: Cart management

  Scenario: Add featured to cart
    Given the web browser is at home page of store
    When user clicks Add to cart under "iPhone" product on home page
    Then "iPhone" product is in cart
    And cart button is showing correct price

  Scenario: Removing item from cart window
    Given "iPhone" product is in cart
    And the web browser is at home page of store
    When product "iPhone" is deleted from cart window using red X button
    Then cart is displaying 0 items in it
    And cart button is showing price "$0.00"

  Scenario: Removing item from cart page
    Given "iPhone" product is in cart
    And web browser is at shopping cart page
    When user deletes "iPhone" form cart
    Then cart is empty

  Scenario: Add to cart from product page
    Given web browser is at page of "iPhone" product
    When product is added to cart from it's page
    Then "iPhone" product is in cart
    And cart button is showing correct price

  Scenario: Checking shipping price
    Given "iPhone" product is in cart
    And web browser is at shopping cart page
    When user enters shipping destination into shipping estimation tab
    Then message box with shipping methods and prices is displayed

  Scenario: Checking shipping price but now with MacBook
    Given "MacBook" product is in cart
    And web browser is at shopping cart page
    When user enters shipping destination into shipping estimation tab
    Then message box with shipping methods and prices is displayed

  Scenario: Updating quantity from cart
    Given "iPhone" product is in cart
    And web browser is at shopping cart page
    When user updates quantity to "3"
    Then quantity of product is "3"
    And total price is "3" times the previous total

  Scenario: Moving to checkout from cart page
    Given "iPhone" product is in cart
    And web browser is at shopping cart page
    When user clicks Checkout button
    Then the checkout page is displayed

  Scenario: Moving to checkout from cart window
    Given "iPhone" product is in cart
    And the web browser is at home page of store
    When user clicks on checkout in cart window
    Then the checkout page is displayed

  Scenario: Add to cart from category page
    Given web browser is at page displaying 'Cameras' category
    When user adds product "Nikon D300" to cart from category page
    Then "Nikon D300" product is in cart
    And cart button is showing correct price