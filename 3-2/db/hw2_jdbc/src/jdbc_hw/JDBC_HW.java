package jdbc_hw;

import java.sql.DatabaseMetaData;
import java.sql.DriverManager;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Scanner;
import java.util.StringTokenizer;
import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.sql.PreparedStatement;
import javax.swing.JFrame;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

public class JDBC_HW {
	public static void main(String args[]) throws Exception {
		boolean printSwitch = false;
		boolean exe = true;
		boolean showTable = false;

		String user = "hongik";
		String pw = "password";
		String url = "jdbc:oracle:thin:@localhost:1521:system";

		Connection conn = null;
		try {
			Class.forName("oracle.jdbc.driver.OracleDriver");
			conn = DriverManager.getConnection(url, user, pw);

			System.out.println("DB Connection Success.\n");
		} catch (ClassNotFoundException cnfe) {
			System.out.println("DB Driver Loading Failed :" + cnfe.toString());
		} catch (SQLException sqle) {
			System.out.println("DB Connection Failed : " + sqle.toString());
		} catch (Exception e) {
			System.out.println("Unkonwn error");
			e.printStackTrace();
		}

		Statement st = conn.createStatement();
		DatabaseMetaData meta = conn.getMetaData();

		Scanner scan = new Scanner(System.in);
		while (true) {
			System.out.println("Input Parameter 1 or 2 (Exit : 0) ");
			System.out.println("(1: Insert Records(Like \"1 example.txt\") 2: Show Table(Like \"2 example\")");
			int inst = scan.nextInt();
			if (inst == 1) {
				//
				// File Name Parsing
				String txtFileName = scan.nextLine().trim();
				StringTokenizer dotToken = new StringTokenizer(txtFileName, ".");
				String fileName = dotToken.nextToken();
				// Get Schemas
				ResultSet rs = st.executeQuery("select * from " + fileName);
				ResultSetMetaData rsmd = rs.getMetaData();
				int columnCount = rsmd.getColumnCount();
				boolean[] dateCheck = new boolean[columnCount];
				String insertSchemas = "(";
				for (int i = 1; i <= columnCount; i++) {
					insertSchemas += rsmd.getColumnName(i);
					insertSchemas += ",";
					//
					// Date는 따로 처리해줘야한다.
					if (rsmd.getColumnTypeName(i) == "DATE") {
						dateCheck[i - 1] = true;
					} else {
						dateCheck[i - 1] = false;
					}
				}
				int length = insertSchemas.length();
				insertSchemas = insertSchemas.substring(0, length - 1) + ")";
				String query = "INSERT INTO " + fileName + insertSchemas + " VALUES (";
				//
				// txt file open
				File f = new File(txtFileName);
				FileReader fr = new FileReader(f);
				BufferedReader br = new BufferedReader(fr);
				String line = "";
				// 한 줄 씩 읽기
				while ((line = br.readLine()) != null) {
					String queryIter = query;
					String content = "";
					// comma parsing
					String[] token = line.split(",", columnCount);
					int tokenLength = token.length;
					for (int i = 0; i < tokenLength; i++) {
						if (dateCheck[i] == false) {
							content += "\'" + token[i] + "\'";
						} else {
							content += "TO_DATE(\'" + token[i] + "\', \'YYYY-MM-DD HH24:MI\')";
						}
						if (i != tokenLength - 1) {
							content += ",";
						}
					}
					queryIter += (content + ")");
					if (printSwitch) {
						System.out.println(queryIter);
					}
					if (exe) {
						PreparedStatement pst = conn.prepareStatement(queryIter);
						rs = pst.executeQuery();
						pst.close();
					}
				}
				br.close();
			} else if (inst == 2) {
				// File Name input
				String fileName = scan.nextLine().trim();

				ResultSet rs2 = st.executeQuery("select count(*) from " + fileName);
				rs2.next();
				int rowCount = (rs2.getInt(1));
				rs2.close();

				// Query.
				ResultSet rs = st.executeQuery("SELECT * FROM " + fileName);
				if (rs.next()) {
					if (showTable) {
						// Show JTable.
						ResultSetMetaData rsmd = rs.getMetaData();
						int columnCount = rsmd.getColumnCount();
						// Get Attribute Name.
						String[] attribute = new String[columnCount];
						for (int i = 0; i < columnCount; i++) {
							attribute[i] = rsmd.getColumnName(i + 1);
						}
						// JTable Construct.
						JFrame frame = new JFrame("Query Result");
						DefaultTableModel model = new DefaultTableModel(attribute, 0);
						JTable table = new JTable(model);
						model.addRow(attribute);

						do {
							String[] record = new String[columnCount];
							for (int i = 1; i <= columnCount; i++) {
								record[i - 1] = rs.getObject(i).toString();
							}
							model.addRow(record);

						} while (rs.next());

						frame.add(table);
						frame.pack();
						frame.setVisible(true);
						System.out.println("Look at Pop-up Tables");
					} else {
						// Console Print.
						ResultSetMetaData rsmd = rs.getMetaData();
						int columnCount = rsmd.getColumnCount();
						// maxLength Array.
						int[] maxLength = new int[columnCount];
						for (int i = 0; i < columnCount; i++) {
							maxLength[i] = 0;
						}
						String[][] printStr = new String[rowCount + 1][columnCount];
						for (int i = 0; i < columnCount; i++) {
							printStr[0][i] = rsmd.getColumnLabel(i + 1);
						}

						int r = 1;
						do {
							for (int j = 0; j < columnCount; j++) {
								if (rs.getObject(j + 1) != null) {
									printStr[r][j] = rs.getObject(j + 1).toString();
								} else {
									printStr[r][j] = "null";
								}
								maxLength[j] = Math.max(maxLength[j], printStr[r][j].length());
							}
							r++;
						} while (rs.next());

						for (int i = 0; i < rowCount + 1; i++) {
							for (int j = 0; j < columnCount; j++) {
								System.out.format("%-" + (maxLength[j] + 10) + "s", printStr[i][j]);
							}
							System.out.println();
						}
						System.out.println();
					}
				} else {
					// empty Table
					System.out.println("No record inserted yet.\n");
				}
			} else if (inst == 0) {
				break;
			} else {
				System.out.println("Wrong Input. Input Again.");
				continue;
			}
		}
	}
}