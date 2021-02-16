Feature: Comparing product

  Scenario: Adding to comparison featured products
    Given the web browser is at home page of store
    When the user selects "iPhone" for comparison
    And the user selects "MacBook" for comparison
    And  the user clicks 'product comparison'
    Then result page comparing "iPhone" and "MacBook" is displayed

  Scenario: Adding to comparison from category page
    Given the web browser is at category page of "MP3 Players" products
    When the user selects "iPod Shuffle" for comparison
    And  the user clicks 'product comparison'
    Then result page comparing "iPod Shuffle" is displayed

  Scenario: Adding to comparison from product page
    Given the web browser is at product page of "iPhone"
    When the user adds "iPhone" to comparison
    And  the user clicks 'product comparison'
    Then result page comparing "iPhone" is displayed

  Scenario: Add to cart from comparison
    Given the web browser is at page comparing "iPhone" and "MacBook"
    When add to cart button is clicked under "iPhone" product
    Then "iPhone" product is in cart
    And cart button is showing price "$101.00"

  Scenario: Removing from comparison
    Given the web browser is at page comparing "iPhone" and "MacBook"
    When user removes "iPhone" from comparison
    Then comparison is displaying only "MacBook"