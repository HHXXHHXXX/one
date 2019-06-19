package com.opensource;

import org.junit.Assert;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

class jmeterTest {
	@BeforeAll
	static void setUpBeforeClass() throws Exception {
	}

	@AfterAll
	static void tearDownAfterClass() throws Exception {
	}

	@Test
	void testGetName() {
		 Assert.assertEquals("111", "111");
	}



	@Test
	void testGetFrienf() {
		 Assert.assertEquals("111", "111");
	}

	@Test
	void testSetFrienf() {
		 Assert.assertEquals("111", "111");
	}

	@Test
	void testSetName() {
		 Assert.assertEquals("111", "111");
	}

}
