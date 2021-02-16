Feature: Comparing product

  Scenario: Adding to comparison featured products
    Given the web browser is at home page of store
    When the user selects "MacBook" for comparison from home page
    And the user selects "iPhone" for comparison from home page
    And  the user clicks 'product comparison'
    Then result page comparing "MacBook" and "iPhone" is displayed

  Scenario: Adding to comparison from category page
    Given web browser is at 'MP3 Players' category page
    When the user selects "iPod Shuffle" for comparison from category page
    And  the user clicks 'product comparison'
    Then result page comparing "iPod Shuffle" is displayed

  Scenario: Adding to comparison from product page
    Given web browser is at page of "iPhone" product
    When the user adds product to comparison from it's page
    And  the user clicks 'product comparison'
    Then result page comparing "iPhone" is displayed

  Scenario: Add to cart from comparison
    Given the web browser is at page comparing "iPhone" and "MacBook"
    When add to cart button is clicked under "iPhone" product
    Then "iPhone" product is in cart
    And cart button is showing price "$123.20"

  Scenario: Removing from comparison
    Given the web browser is at page comparing "iPhone" and "MacBook"
    When user removes "iPhone" from comparison
    Then comparison is displaying only "MacBook"