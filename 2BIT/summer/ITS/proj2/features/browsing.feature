Feature: Browsing store

  Scenario: Searching product with search bar
    Given the web browser is at home page of store
    When the user enters "iPhone" to the search bar
    Then products related to the "iPhone" are shown on result page

  Scenario: Currency change
    Given the web browser is at home page of store
    When user changes currency to "euro"
    Then currency at cart window will be "euro"
    And currency of prices of featured products will be "euro"

  Scenario: Displaying category
    Given the web browser is at home page of store
    When user chooses "Desktop" category from menu
    Then page containing "Desktop" products is displayed

  Scenario: Displaying category - Working
    Given the web browser is at home page of store
    When user chooses "Tablets" category from menu
    Then page containing "Tablets" products is displayed

  Scenario: Displaying brands
    Given the web browser is at home page of store
    When users clicks 'Brands' in footer
    Then page showing all brands is displayed

  Scenario: Displaying brand products
    Given the web browser is at Brands page
    When users chooses "Apple" brand
    Then page showing all "Apple" products is displayed

  Scenario: Sending contact form
    Given web browser is at 'Contact us' page
    When user correctly submits contact form
    Then page showing success is displayed

  Scenario: Going to home page
    Given web browser is at 'Desktop' category page
    When user clicks logo of the store
    Then home page of store is displayed

  Scenario: Sorting
    Given web browser is at 'Phones & PDAs' category page
    When user chooses sorting by "ascending price"
    Then products are sorted by "ascending price"

  Scenario: Sorting by descending name
    Given web browser is at 'Phones & PDAs' category page
    When user chooses sorting by "descending name"
    Then products are sorted by "descending name"